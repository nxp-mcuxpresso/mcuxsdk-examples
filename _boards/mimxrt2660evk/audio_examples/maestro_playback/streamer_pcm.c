/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Board-local PCM interface for the RT2660 EVK maestro_playback example.
 *
 * The shared streamer_pcm.c under examples/audio_examples/maestro_playback/RT
 * targets the RT10xx family (SAI1, DMAMUX, IOMUXC_GPR MCLK gate, i.MXRT10xx
 * clock_audio_pll_config_t). RT2660 has AUDIO__SAI_0 driving the WM8962 codec
 * on the EVK through AUDIO__EDMA3, and the master clock is gated via MODCON
 * rather than IOMUXC_GPR — so we ship a dedicated implementation here.
 */

#include "board.h"
#include "app_definitions.h"
#include "app.h"
#include "streamer_pcm.h"

#include "fsl_codec_common.h"
#include "fsl_debug_console.h"

#if (defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
#include "fsl_cache.h"
#endif

#define ALL_CHANNELS (0xFFU)

extern codec_config_t boardCodecConfig;
extern codec_handle_t codecHandle;

AT_NONCACHEABLE_SECTION_INIT(static pcm_rtos_t pcmHandle) = {0};

/* fsl_llc.h defines a non-static function in the header, so it cannot be
 * included from a second translation unit; forward-declare the one entry point
 * used here. 128 B is the sole LLC line size (kLLC_CacheLineSize128B). */
extern status_t LLC_CleanInvalidateCacheAtAddressRange(LLC_Type *base, uint32_t address, uint16_t sizeInCacheLines);
#define PCM_LLC_LINE_BYTES (128U)

/* Write back the LLC lines backing [address, address+sizeByte). The driver's
 * LLC_CleanInvalidateCacheByRange() cannot resolve the 0x88000000 PSRAM alias
 * on RT2663 (LLC_PHYMEM_BASE_ALIAS_COUNT undefined), so drive the address-range
 * primitive directly. */
static void pcm_llc_writeback(uint32_t address, uint32_t sizeByte)
{
    uint32_t start = address & ~(PCM_LLC_LINE_BYTES - 1U);
    uint32_t end   = (address + sizeByte - 1U) & ~(PCM_LLC_LINE_BYTES - 1U);
    (void)LLC_CleanInvalidateCacheAtAddressRange(CMPT__LLC, start,
                                                 (uint16_t)((end - start) / PCM_LLC_LINE_BYTES + 1U));
}

/*! @brief SAI transmit FIFO-error handler.
 *
 * Runs at ISR priority when the WM8962 codec falls behind the SAI transmitter
 * (Tx FIFO underrun); we clear the error latch and soft-reset the FIFO so the
 * next EDMA burst can proceed cleanly.
 */
static void SAI_UserTxIRQHandler(void)
{
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    SAI_TxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);

    SDK_ISR_EXIT_BARRIER;
}

/*! @brief RT2660 AUDIO__SAI_0 interrupt handler. */
void AUDIO_SAI0_IRQHandler(void)
{
    if ((DEMO_SAI->TCSR & kSAI_FIFOErrorFlag) != 0U)
    {
        SAI_UserTxIRQHandler();
    }
}

/*! @brief SAI EDMA transmit callback — releases the streamer write-thread. */
static void saiTxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    BaseType_t reschedule = -1;
    xSemaphoreGiveFromISR(pcmHandle.semaphoreTX, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

void streamer_pcm_init(void)
{
    edma_config_t dmaConfig;

    NVIC_SetPriority(DEMO_SAI_IRQ, 5U);

    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_DMA, &dmaConfig);
    EDMA_CreateHandle(&(pcmHandle.dmaTxHandle), DEMO_DMA, DEMO_TX_EDMA_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_DMA, DEMO_TX_EDMA_CHANNEL, DEMO_SAI_TX_EDMA_CHANNEL);
#endif

    SAI_Init(DEMO_SAI);

    /* Assert SAI0 MCLK output (I2S_MCR[MOE]) after SAI_Init so the on-board
     * WM8962 codec receives SYSCLK; without it the codec stays powered down. */
    BOARD_MasterClockConfig();

    EnableIRQ(DEMO_SAI_IRQ);
}

int streamer_pcm_open(uint32_t num_buffers)
{
    pcmHandle.semaphoreTX = xSemaphoreCreateBinary();
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &pcmHandle.saiTxHandle, saiTxCallback, (void *)&pcmHandle,
                                   &pcmHandle.dmaTxHandle);
    return 0;
}

void streamer_pcm_close(void)
{
    /* Stop playback; flushes the SAI transmit buffers. */
    SAI_TransferTerminateSendEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle));
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_DMA, DEMO_TX_EDMA_CHANNEL, DEMO_SAI_TX_EDMA_CHANNEL);
#endif
    vSemaphoreDelete(pcmHandle.semaphoreTX);
}

int streamer_pcm_write(uint8_t *data, uint32_t size)
{
    uint32_t remainder;

    /* Ensure write size is a multiple of 32/64, otherwise EDMA will assert. */
    remainder = size % ((pcmHandle.bit_width == 16U) ? 32U : 64U);
    assert(size >= remainder); /* INT30-C: prevent unsigned underflow */
    pcmHandle.saiTx.dataSize = size - remainder;
    pcmHandle.saiTx.data     = data;

    /* Split the first transfer into two to ensure the continuity. */
    if (pcmHandle.isFirstTx)
    {
        pcmHandle.saiTx.dataSize /= 2U;
    }

#if (defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
    DCACHE_CleanByRange((uint32_t)(pcmHandle.saiTx.data), pcmHandle.saiTx.dataSize);
#endif
    /* TX buffers are in LLC-cached PSRAM but the EDMA master bypasses the LLC;
     * write each back before queuing or the DMA replays stale PSRAM (90 ms tick). */
    pcm_llc_writeback((uint32_t)(pcmHandle.saiTx.data), pcmHandle.saiTx.dataSize);
    while (SAI_TransferSendEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle), &(pcmHandle.saiTx)) == kStatus_SAI_QueueFull)
    {
        if (xSemaphoreTake(pcmHandle.semaphoreTX, portMAX_DELAY) != pdTRUE)
        {
            return -1;
        }
    }

    if (pcmHandle.isFirstTx)
    {
        pcmHandle.saiTx.data += pcmHandle.saiTx.dataSize;
        pcm_llc_writeback((uint32_t)(pcmHandle.saiTx.data), pcmHandle.saiTx.dataSize);
        SAI_TransferSendEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle), &(pcmHandle.saiTx));
        pcmHandle.isFirstTx = 0U;
    }
    return 0;
}

static sai_sample_rate_t _pcm_map_sample_rate(uint32_t sample_rate)
{
    switch (sample_rate)
    {
        case 8000U:  return kSAI_SampleRate8KHz;
        case 11025U: return kSAI_SampleRate11025Hz;
        case 12000U: return kSAI_SampleRate12KHz;
        case 16000U: return kSAI_SampleRate16KHz;
        case 24000U: return kSAI_SampleRate24KHz;
        case 22050U: return kSAI_SampleRate22050Hz;
        case 32000U: return kSAI_SampleRate32KHz;
        case 44100U: return kSAI_SampleRate44100Hz;
        case 96000U: return kSAI_SampleRate96KHz;
        case 48000U:
        default:     return kSAI_SampleRate48KHz;
    }
}

static sai_word_width_t _pcm_map_word_width(uint32_t bit_width)
{
    switch (bit_width)
    {
        case 8U:  return kSAI_WordWidth8bits;
        case 16U: return kSAI_WordWidth16bits;
        case 24U: return kSAI_WordWidth24bits;
        case 32U: return kSAI_WordWidth32bits;
        default:  return kSAI_WordWidth16bits;
    }
}

static sai_mono_stereo_t _pcm_map_channels(uint8_t num_channels)
{
    return (num_channels >= 2U) ? kSAI_Stereo : kSAI_MonoLeft;
}

int streamer_pcm_setparams(
    uint32_t sample_rate, uint32_t bit_width, uint8_t num_channels, bool transfer, bool dummy_tx, int volume)
{
    sai_transfer_format_t format = {0};
    sai_transceiver_t saiConfig;
    uint32_t masterClockHz = 0U;

    pcmHandle.isFirstTx       = transfer ? 1U : pcmHandle.isFirstTx;
    pcmHandle.sample_rate     = sample_rate;
    pcmHandle.bit_width       = bit_width;
    pcmHandle.num_channels    = num_channels;
    pcmHandle.dummy_tx_enable = dummy_tx;

    masterClockHz = (uint32_t)streamer_set_master_clock(sample_rate);

    format.channel       = 0U;
    format.bitWidth      = _pcm_map_word_width(bit_width);
    format.sampleRate_Hz = _pcm_map_sample_rate(sample_rate);
    format.stereo        = _pcm_map_channels(num_channels);
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    format.masterClockHz = masterClockHz;
#endif

    if (num_channels > 2U)
    {
        PRINTF("\r\nWM8962 codec does not support more than 2 channels.\r\n\r\n");
        return -1;
    }

    SAI_GetClassicI2SConfig(&saiConfig, _pcm_map_word_width(bit_width), format.stereo, 1U << DEMO_SAI_CHANNEL);
    saiConfig.syncMode    = DEMO_SAI_TX_SYNC_MODE;
    saiConfig.masterSlave = DEMO_SAI_MASTER_SLAVE;

    SAI_TransferTerminateSendEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle));
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle), &saiConfig);
    SAI_TxSetBitClockRate(DEMO_SAI, masterClockHz, _pcm_map_sample_rate(sample_rate),
                          _pcm_map_word_width(pcmHandle.bit_width),
                          (pcmHandle.num_channels == 1U) ? 2U : pcmHandle.num_channels);
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);

    /* Bring the codec (re-)up for the new format. */
    CODEC_Init(&codecHandle, &boardCodecConfig);
    CODEC_SetFormat(&codecHandle, masterClockHz, format.sampleRate_Hz, format.bitWidth);

    streamer_pcm_set_volume(volume);

    return 0;
}

void streamer_pcm_getparams(uint32_t *sample_rate, uint32_t *bit_width, uint8_t *num_channels)
{
    *sample_rate  = pcmHandle.sample_rate;
    *bit_width    = pcmHandle.bit_width;
    *num_channels = pcmHandle.num_channels;
}

int streamer_pcm_mute(bool mute)
{
    CODEC_SetMute(&codecHandle, ALL_CHANNELS, mute);
    return 0;
}

int streamer_pcm_set_volume(int volume)
{
    int channel;

    switch (pcmHandle.num_channels)
    {
        case 0:
            return 0;
        case 1:
            channel = kCODEC_PlayChannelHeadphoneLeft;
            break;
        case 2:
        default:
            channel = kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight;
            break;
    }

    if (volume <= 0)
    {
        CODEC_SetMute(&codecHandle, ALL_CHANNELS, true);
    }
    else
    {
        CODEC_SetVolume(&codecHandle, channel,
                        (uint32_t)((volume > CODEC_VOLUME_MAX_VALUE) ? CODEC_VOLUME_MAX_VALUE : volume));
    }

    return 0;
}

int streamer_set_master_clock(uint32_t sample_rate)
{
    /* Retune the SAI0 MCLK0 root divider based on family (48 kHz vs 44.1 kHz).
     * The AUDIO PLL itself is fixed at 49.152 MHz by ConfigCGUAna(), so we
     * only adjust the root divider (and only if the target family changes).
     * For 44.1 kHz / 22.05 kHz / 11.025 kHz the ideal MCLK is 22.5792 MHz —
     * the WM8962 tolerates the 24.576 MHz reference with its internal FLL, so
     * we keep div=2 for both families and let the codec generate the SYSCLK.
     */
    clock_root_config_t saiRootCfg = {
        .mux = kCLOCK_SAI0_MCLK0_ClockRoot_AUDIOPLL,
        .div = 2U,
    };
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_sai0_mclk0, &saiRootCfg);

    (void)sample_rate;
    return (int)DEMO_SAI_CLK_FREQ;
}
