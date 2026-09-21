/*
 * Copyright 2021-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Board-local streamer_pcm.c for RT2660 maestro_usb_mic. Adapted from
 * mcuxsdk/examples/audio_examples/maestro_usb_mic/RT/streamer_pcm.c with two
 * RT2660-specific changes:
 *  1. Removed CLOCK_InitAudioPll(clock_audio_pll_config_t ...) block in
 *     streamer_pcm_setparams — RT2660 uses clock_cguana_avpll_config_t and
 *     the AUDIOPLL is already brought up by BOARD_InitBootClocks; only the
 *     SAI root divider needs adjustment (BOARD_MASTER_CLOCK_CONFIG / bit
 *     clock rate calls handle that).
 *  2. Removed SAI1_IRQHandler — RT2660's SAI0 error IRQ (if wired) would use
 *     AUDIO_SAI0_IRQHandler, but the maestro USB mic path does not need a
 *     custom error handler.
 */

#include "board.h"
#include "streamer_pcm.h"
#include "fsl_codec_common.h"
#include "app_definitions.h"
#include "app.h"
#if (defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
#include "fsl_cache.h"
#endif
#if (defined(DEMO_CODEC_WM8962) && (DEMO_CODEC_WM8962 == 1))
extern codec_config_t boardCodecConfig;
#endif
#include "audio_microphone.h"
#include "fsl_sai.h"

AT_NONCACHEABLE_SECTION_INIT(static pcm_rtos_t pcmHandle) = {0};
extern codec_handle_t codecHandle;
extern usb_audio_microphone_struct_t s_audioMicrophone;
extern uint8_t audioMicDataBuff[AUDIO_MICROPHONE_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE];

/* Continuous double-buffered SAI RX: keep NUM_RX_BUFFERS transfers queued so
 * the EDMA never idles between the 1 ms frames. A single-buffer stop/start
 * scheme dropped the samples that arrived while no transfer was armed, causing
 * a discontinuity every ~1-2 ms. RX_CHUNK = one 1 ms mono 48 kHz frame. */
#define NUM_RX_BUFFERS (4U)
#define RX_CHUNK       (FS_ISO_IN_ENDP_PACKET_SIZE) /* 96 B = 1 ms @ 48k mono 16-bit */
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_rxBuffers[NUM_RX_BUFFERS][RX_CHUNK], 32);
static volatile uint32_t s_rxHead = 0U; /* next buffer the DMA will fill  */
static volatile uint32_t s_rxTail = 0U; /* next buffer the reader consumes */


/*! @brief SAI EDMA receive callback */
static void saiRxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    BaseType_t reschedule = pdFALSE;
    xSemaphoreGiveFromISR(pcmHandle.semaphoreRX, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}


/*! @brief SAI RX error IRQ handler.
 *
 * RT2660 routes SAI0 to AUDIO_SAI0_IRQn, which streamer_pcm_init() enables. The
 * RX FIFO error interrupt therefore needs a handler that clears the error flag;
 * without it the interrupt re-asserts continuously and starves every task, so
 * the USB device never gets a chance to run and the board fails to enumerate.
 * This mirrors SAI1_IRQHandler in the shared RT/streamer_pcm.c (reference board
 * uses SAI1; RT2660 uses SAI0). */
void AUDIO_SAI0_IRQHandler(void)
{
    if (DEMO_SAI->RCSR & kSAI_FIFOErrorFlag)
    {
        /* Clear the FEF (Rx FIFO overflow) flag and reset the FIFO. */
        SAI_RxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
        SAI_RxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);
    }
    SDK_ISR_EXIT_BARRIER;
}

void streamer_pcm_init(void)
{
    edma_config_t dmaConfig;

    /* Interrupt and DMA initialization */
    NVIC_SetPriority(DEMO_I2C_IRQ, 5);
    NVIC_SetPriority(DEMO_SAI_IRQ, 5U);
    NVIC_SetPriority(DEMO_DMA_RX_IRQ, 4U);

    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_DMA, &dmaConfig);
    /* Create DMA handle. */
    EDMA_CreateHandle(&pcmHandle.dmaRxHandle, DEMO_DMA, DEMO_DMA_RX_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX && defined(DEMO_SAI_RX_SOURCE)
    EDMA_SetChannelMux(DEMO_DMA, DEMO_DMA_RX_CHANNEL, DEMO_SAI_RX_SOURCE);
#endif
    /* SAI init */
    SAI_Init(DEMO_SAI);

    /* Assert SAI0 MCLK output (I2S_MCR[MOE]) after SAI_Init so the on-board
     * WM8962 codec receives SYSCLK; without it the codec stays powered down. */
    BOARD_MasterClockConfig();

    /* Counting semaphore: several queued RX buffers can complete before
     * streamer_pcm_read() runs; a binary one would lose completions and
     * desync the ring tail. */
    pcmHandle.semaphoreRX = xSemaphoreCreateCounting(NUM_RX_BUFFERS, 0);

    EnableIRQ(DEMO_SAI_IRQ);
}


int streamer_pcm_rx_open(uint32_t num_buffers)
{
    SAI_TransferRxCreateHandleEDMA(DEMO_SAI, &(pcmHandle.saiRxHandle), saiRxCallback, (void *)&pcmHandle,
                                   &(pcmHandle.dmaRxHandle));
    s_rxHead        = 0U;
    s_rxTail        = 0U;
    pcmHandle.isFirstRx = 1U;
    return 0;
}


void streamer_pcm_rx_close(void)
{
    SAI_TransferTerminateReceiveEDMA(DEMO_SAI, &(pcmHandle.saiRxHandle));
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX && defined(DEMO_SAI_RX_SOURCE)
    EDMA_SetChannelMux(DEMO_DMA, DEMO_DMA_RX_CHANNEL, DEMO_SAI_RX_SOURCE);
#endif
    vSemaphoreDelete(pcmHandle.semaphoreRX);
}

int streamer_pcm_write(uint8_t *data, uint32_t size)
{
#if (defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
    DCACHE_CleanInvalidateByRange((uint32_t)data, size);
#endif
    if (s_audioMicrophone.start)
    {
        memcpy(audioMicDataBuff + s_audioMicrophone.tdWriteNumber, data, size);
        s_audioMicrophone.tdWriteNumber += size;
        if (s_audioMicrophone.tdWriteNumber >= AUDIO_MICROPHONE_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE)
        {
            s_audioMicrophone.tdWriteNumber = 0;
        }
    }

    if (pcmHandle.dummy_tx_enable)
        SAI_TxEnable(DEMO_SAI, true);

    return 0;
}

/* Queue one internal RX buffer into the SAI EDMA driver. */
static void streamer_pcm_rx_submit(uint32_t idx, uint32_t bytes)
{
    sai_transfer_t xfer;
    xfer.data     = s_rxBuffers[idx];
    xfer.dataSize = bytes;
    (void)SAI_TransferReceiveEDMA(DEMO_SAI, &(pcmHandle.saiRxHandle), &xfer);
}

int streamer_pcm_read(uint8_t *data, uint32_t size)
{
    /* EDMA requires the transfer size be a multiple of the burst/FIFO width. */
    uint32_t bytes = size - (size % 32);
    if (bytes > RX_CHUNK)
    {
        bytes = RX_CHUNK;
    }

    if (pcmHandle.isFirstRx)
    {
        /* Prime the queue: submit all buffers up front so the SAI EDMA driver
         * always has the next transfer chained and the DMA never idles between
         * frames (the root cause of the periodic ~1-2 ms dropouts). */
        for (uint32_t i = 0U; i < NUM_RX_BUFFERS; i++)
        {
            streamer_pcm_rx_submit(i, bytes);
        }
        s_rxHead            = 0U;
        s_rxTail            = 0U;
        pcmHandle.isFirstRx = 0;
    }

    /* Wait for the oldest queued buffer to complete. */
    if (xSemaphoreTake(pcmHandle.semaphoreRX, portMAX_DELAY) != pdTRUE)
        return -1;

    uint32_t idx = s_rxTail;
#if (defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
    DCACHE_InvalidateByRange((uint32_t)s_rxBuffers[idx], bytes);
#endif
    memcpy(data, s_rxBuffers[idx], bytes);

    /* Re-queue the just-consumed buffer to keep the ring full. */
    streamer_pcm_rx_submit(idx, bytes);
    s_rxTail = (s_rxTail + 1U) % NUM_RX_BUFFERS;

    return 0;
}


static sai_sample_rate_t _pcm_map_sample_rate(uint32_t sample_rate)
{
    switch (sample_rate)
    {
        case 8000:  return kSAI_SampleRate8KHz;
        case 11025: return kSAI_SampleRate11025Hz;
        case 12000: return kSAI_SampleRate12KHz;
        case 16000: return kSAI_SampleRate16KHz;
        case 24000: return kSAI_SampleRate24KHz;
        case 22050: return kSAI_SampleRate22050Hz;
        case 32000: return kSAI_SampleRate32KHz;
        case 44100: return kSAI_SampleRate44100Hz;
        case 48000:
        default:    return kSAI_SampleRate48KHz;
    }
}

static sai_word_width_t _pcm_map_word_width(uint32_t bit_width)
{
    switch (bit_width)
    {
        case 8:  return kSAI_WordWidth8bits;
        case 16: return kSAI_WordWidth16bits;
        case 24: return kSAI_WordWidth24bits;
        case 32: return kSAI_WordWidth32bits;
        default: return kSAI_WordWidth16bits;
    }
}

static sai_mono_stereo_t _pcm_map_channels(uint8_t num_channels)
{
    if (num_channels >= 2)
        return kSAI_Stereo;
    else
        return kSAI_MonoRight;
}

int streamer_pcm_setparams(
    uint32_t sample_rate, uint32_t bit_width, uint8_t num_channels, bool transfer, bool dummy_tx, int volume)
{
    sai_transfer_format_t format = {0};
    sai_transceiver_t saiConfig;
    uint32_t masterClockHz = 0U;

    pcmHandle.isFirstRx    = transfer ? pcmHandle.isFirstRx : 1U;
    pcmHandle.sample_rate  = sample_rate;
    pcmHandle.bit_width    = bit_width;
    pcmHandle.num_channels = num_channels;
    pcmHandle.dummy_tx_enable |= dummy_tx;

    /*
     * RT2660: AUDIOPLL is configured by BOARD_InitBootClocks
     * (clock_cguana_avpll_config_t). Do not reinitialize the PLL here.
     */

#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
    masterClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
#else
    masterClockHz = DEMO_SAI_CLK_FREQ;
#endif

    format.channel       = 0U;
    format.bitWidth      = _pcm_map_word_width(bit_width);
    format.sampleRate_Hz = _pcm_map_sample_rate(sample_rate);
    format.stereo        = _pcm_map_channels(num_channels);
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    format.masterClockHz = masterClockHz;
#endif

    if (!transfer)
    {
        SAI_GetClassicI2SConfig(&saiConfig, _pcm_map_word_width(bit_width), format.stereo, 1U << DEMO_SAI_CHANNEL);
        saiConfig.bitClock.bclkSource = (sai_bclk_source_t)DEMO_SAI_CLOCK_SOURCE;
        saiConfig.syncMode            = DEMO_SAI_RX_SYNC_MODE;
        saiConfig.masterSlave         = DEMO_SAI_MASTER_SLAVE;

        SAI_TransferRxSetConfigEDMA(DEMO_SAI, &(pcmHandle.saiRxHandle), &saiConfig);
        SAI_RxSetBitClockRate(DEMO_SAI, masterClockHz, _pcm_map_sample_rate(sample_rate),
                              _pcm_map_word_width(bit_width), DEMO_CHANNEL_NUM);

        if (dummy_tx)
        {
            saiConfig.syncMode    = DEMO_SAI_TX_SYNC_MODE;
            saiConfig.masterSlave = DEMO_SAI_MASTER_SLAVE;
            SAI_TransferTxSetConfigEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle), &saiConfig);
            SAI_TxSetBitClockRate(DEMO_SAI, masterClockHz, _pcm_map_sample_rate(sample_rate),
                                  _pcm_map_word_width(bit_width), DEMO_CHANNEL_NUM);
            SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
        }

        SAI_RxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);

        BOARD_MASTER_CLOCK_CONFIG();
    }

    streamer_pcm_mute(true);
#if (defined(DEMO_CODEC_WM8962) && (DEMO_CODEC_WM8962 == 1))
    CODEC_Init(&codecHandle, &boardCodecConfig);
#endif
    CODEC_SetFormat(&codecHandle, masterClockHz, format.sampleRate_Hz, format.bitWidth);

    streamer_pcm_mute(false);

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
    status_t ret;

    ret = CODEC_SetPower(&codecHandle, kCODEC_ModuleLinein, !mute);
    if (ret != kStatus_Success)
    {
        return 1;
    }

    return 0;
}
