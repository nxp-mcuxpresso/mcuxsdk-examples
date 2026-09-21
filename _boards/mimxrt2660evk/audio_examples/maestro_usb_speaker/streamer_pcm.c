/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Board-local streamer_pcm.c for RT2660 maestro_usb_speaker. Adapted from
 * mcuxsdk/examples/audio_examples/maestro_usb_speaker/RT/streamer_pcm.c with
 * two RT2660-specific changes:
 *  1. Removed CLOCK_InitAudioPll(clock_audio_pll_config_t ...) block in
 *     streamer_pcm_setparams — RT2660 uses clock_cguana_avpll_config_t and
 *     the AUDIOPLL is already brought up by BOARD_InitBootClocks; only the
 *     SAI root divider needs adjustment.
 *  2. Replaced SAI1_IRQHandler with AUDIO_SAI0_IRQHandler — RT2660 routes SAI0
 *     to AUDIO_SAI0_IRQn; the TX FIFO-error interrupt armed in
 *     streamer_pcm_setparams() needs a handler that clears FEF, otherwise a TX
 *     underrun storms the interrupt and permanently stalls playback.
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
#include "audio_speaker.h"
#include "fsl_sai.h"

AT_NONCACHEABLE_SECTION_INIT(static pcm_rtos_t pcmHandle) = {0};
extern codec_handle_t codecHandle;
extern usb_audio_speaker_struct_t g_UsbDeviceAudioSpeaker;
extern uint8_t audioPlayDataBuff[AUDIO_SPEAKER_DATA_WHOLE_BUFFER_COUNT_NORMAL * AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME];
static uint8_t audioPlayDMATempBuff[AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME] = {0};

/*! @brief SAI EDMA transmit callback */
static void saiTxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    BaseType_t reschedule = -1;
    xSemaphoreGiveFromISR(pcmHandle.semaphoreTX, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

/*! @brief SAI TX error IRQ handler.
 *
 * RT2660 routes SAI0 to AUDIO_SAI0_IRQn, which streamer_pcm_init() enables and
 * streamer_pcm_setparams() arms via kSAI_FIFOErrorInterruptEnable. A TX FIFO
 * underrun (FEF) sets an error flag that this handler must clear; otherwise the
 * interrupt re-asserts continuously, the SAI TX never recovers, and audio stops
 * after the first few buffers. Mirrors the RX handler in maestro_usb_mic. */
void AUDIO_SAI0_IRQHandler(void)
{
    if (DEMO_SAI->TCSR & kSAI_FIFOErrorFlag)
    {
        /* Clear the FEF (Tx FIFO underrun) flag and reset the FIFO. */
        SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
        SAI_TxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);
    }
    SDK_ISR_EXIT_BARRIER;
}

void streamer_pcm_init(void)
{
    edma_config_t dmaConfig;

    /* Interrupt and DMA initialization */
    NVIC_SetPriority(DEMO_I2C_IRQ, 5);
    NVIC_SetPriority(DEMO_SAI_IRQ, 5U);
    NVIC_SetPriority(DEMO_DMA_TX_IRQ, 4U);

    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_DMA, &dmaConfig);
    /* Create DMA handle. */
    EDMA_CreateHandle(&(pcmHandle.dmaTxHandle), DEMO_DMA, DEMO_DMA_TX_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX && defined(DEMO_SAI_TX_SOURCE)
    EDMA_SetChannelMux(DEMO_DMA, DEMO_DMA_TX_CHANNEL, DEMO_SAI_TX_SOURCE);
#endif
    /* SAI init */
    SAI_Init(DEMO_SAI);

    /* Assert SAI0 MCLK output (I2S_MCR[MOE]) after SAI_Init so the on-board
     * WM8962 codec receives SYSCLK; without it the codec stays powered down. */
    BOARD_MasterClockConfig();

    pcmHandle.semaphoreTX = xSemaphoreCreateBinary();

    EnableIRQ(DEMO_SAI_IRQ);
}

int streamer_pcm_tx_open(uint32_t num_buffers)
{
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle), saiTxCallback, (void *)&pcmHandle,
                                   &(pcmHandle.dmaTxHandle));
    return 0;
}

void streamer_pcm_tx_close(void)
{
    SAI_TransferTerminateSendEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle));
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX && defined(DEMO_SAI_TX_SOURCE)
    EDMA_SetChannelMux(DEMO_DMA, DEMO_DMA_TX_CHANNEL, DEMO_SAI_TX_SOURCE);
#endif
    vSemaphoreDelete(pcmHandle.semaphoreTX);
}

int streamer_pcm_write(uint8_t *data, uint32_t size)
{
    pcmHandle.saiTx.dataSize = size - (size % 32);
    pcmHandle.saiTx.data     = data;

#if (defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
    DCACHE_CleanByRange((uint32_t)(pcmHandle.saiTx.data), pcmHandle.saiTx.dataSize);
#endif

    while (SAI_TransferSendEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle), &(pcmHandle.saiTx)) == kStatus_SAI_QueueFull)
    {
        if (xSemaphoreTake(pcmHandle.semaphoreTX, portMAX_DELAY) != pdTRUE)
        {
            return -1;
        }
    }

    return 0;
}

int streamer_pcm_read(uint8_t *data, uint32_t size)
{
    uint32_t audioSpeakerPreReadDataCount = 0U;
    uint32_t preAudioSendCount            = 0U;
    sai_transfer_t xfer                   = {0};

    if ((USB_AudioSpeakerBufferSpaceUsed() < (g_UsbDeviceAudioSpeaker.audioPlayTransferSize)) &&
        (g_UsbDeviceAudioSpeaker.startPlayFlag == 1U))
    {
        g_UsbDeviceAudioSpeaker.startPlayFlag          = 0;
        g_UsbDeviceAudioSpeaker.speakerDetachOrNoInput = 1;
    }
    if (0U != g_UsbDeviceAudioSpeaker.startPlayFlag)
    {
        USB_DeviceCalculateFeedback();
        xfer.dataSize     = g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        xfer.data         = audioPlayDataBuff + g_UsbDeviceAudioSpeaker.tdReadNumberPlay;
        preAudioSendCount = g_UsbDeviceAudioSpeaker.audioSendCount[0];
        g_UsbDeviceAudioSpeaker.audioSendCount[0] += g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        if (preAudioSendCount > g_UsbDeviceAudioSpeaker.audioSendCount[0])
        {
            g_UsbDeviceAudioSpeaker.audioSendCount[1] += 1U;
        }
        g_UsbDeviceAudioSpeaker.audioSendTimes++;
        g_UsbDeviceAudioSpeaker.tdReadNumberPlay += g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        if (g_UsbDeviceAudioSpeaker.tdReadNumberPlay >= g_UsbDeviceAudioSpeaker.audioPlayBufferSize)
        {
            g_UsbDeviceAudioSpeaker.tdReadNumberPlay = 0;
        }
        audioSpeakerPreReadDataCount = g_UsbDeviceAudioSpeaker.audioSpeakerReadDataCount[0];
        g_UsbDeviceAudioSpeaker.audioSpeakerReadDataCount[0] += g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        if (audioSpeakerPreReadDataCount > g_UsbDeviceAudioSpeaker.audioSpeakerReadDataCount[0])
        {
            g_UsbDeviceAudioSpeaker.audioSpeakerReadDataCount[1] += 1U;
        }
    }
    else
    {
        if (0U != g_UsbDeviceAudioSpeaker.audioPlayTransferSize)
        {
            xfer.dataSize = g_UsbDeviceAudioSpeaker.audioPlayTransferSize;
        }
        else
        {
            xfer.dataSize = AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME / 8U;
        }
        xfer.data = audioPlayDMATempBuff;
    }

#if (defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
    DCACHE_InvalidateByRange((uint32_t)data, size);
#endif
    memcpy(data, xfer.data, size);

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

    pcmHandle.sample_rate  = sample_rate;
    pcmHandle.bit_width    = bit_width;
    pcmHandle.num_channels = num_channels;

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

    if (transfer)
    {
        SAI_TransferTerminateSendEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle));
        SAI_GetClassicI2SConfig(&saiConfig, _pcm_map_word_width(bit_width), format.stereo, 1U << DEMO_SAI_CHANNEL);

        saiConfig.bitClock.bclkSource = (sai_bclk_source_t)DEMO_SAI_CLOCK_SOURCE;
        saiConfig.syncMode            = kSAI_ModeAsync;
        saiConfig.masterSlave         = DEMO_SAI_MASTER_SLAVE;

        SAI_TransferTxSetConfigEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle), &saiConfig);
        SAI_TxSetBitClockRate(DEMO_SAI, masterClockHz, _pcm_map_sample_rate(sample_rate),
                              _pcm_map_word_width(bit_width), DEMO_CHANNEL_NUM);
        SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);

        BOARD_MASTER_CLOCK_CONFIG();

        streamer_pcm_mute(true);
#if (defined(DEMO_CODEC_WM8962) && (DEMO_CODEC_WM8962 == 1))
        CODEC_Init(&codecHandle, &boardCodecConfig);
#endif
        CODEC_SetFormat(&codecHandle, masterClockHz, format.sampleRate_Hz, format.bitWidth);

        streamer_pcm_set_volume(volume);
    }

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
    CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, mute);

    return 0;
}

int streamer_pcm_set_volume(int volume)
{
    if (volume <= 0)
        CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, true);
    else
        CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft,
                        volume > CODEC_VOLUME_MAX_VALUE ? CODEC_VOLUME_MAX_VALUE : volume);

    return 0;
}
