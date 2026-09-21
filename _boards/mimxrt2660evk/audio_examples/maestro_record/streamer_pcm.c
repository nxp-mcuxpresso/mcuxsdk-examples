/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Board-local PCM interface for the RT2660 EVK maestro_record example: SAI0 TX
 * to the WM8962 codec and MICFIL (PDM) capture, both on AUDIO__EDMA3 via the
 * edma4 channel-mux driver. The read/write API contract follows the RT1170
 * reference so the shared app_streamer.c consumer works unchanged.
 */

#include "board.h"
#include "streamer_pcm.h"
#include "app_definitions.h"
#include "app.h"

#include "fsl_codec_common.h"
#include "fsl_wm8962.h"
#include "fsl_debug_console.h"

#if (defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
#include "fsl_cache.h"
#endif

#include "fsl_pdm.h"
#include "fsl_pdm_edma.h"

/* --- state ---------------------------------------------------------------- */
volatile int8_t PDM_started = 0;
volatile int8_t SAI_started = 0;
volatile int rx_data_valid  = 0;
volatile bool rx_sem_take   = false;

#define RECORD_BUFFER_SIZE (3840) /* 16 kHz * 4 B * 2 ch * 30 ms */
#define BUFFER_NUM         (3U)
#define BUFFER_SIZE        (RECORD_BUFFER_SIZE * BUFFER_NUM)
#define ALL_CHANNELS       (0xFFU)

AT_NONCACHEABLE_SECTION_INIT(static pcm_rtos_t pcmHandle) = {0};
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_buffer[BUFFER_SIZE], 32);
extern codec_config_t boardCodecConfig;
extern codec_handle_t codecHandle;

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

/* Queue for streamer-provided read buffer addresses. */
typedef struct
{
    int num_buffers;
    int head;
    int tail;
    int count;
    uint8_t *buff_addr[SAI_XFER_QUEUE_SIZE + 2];
} Streamer_buff_addr_T;

static Streamer_buff_addr_T streamer_buff_addr = {
    .num_buffers = SAI_XFER_QUEUE_SIZE + 2,
    .head        = 0,
    .tail        = 0,
};

/* The EDMA scatter-gather TCD ring must be reachable by EDMA3; use non-cacheable SRAM. */
AT_NONCACHEABLE_SECTION_ALIGN(edma_tcd_t s_edmaTcd[BUFFER_NUM], 32U);

static pdm_edma_transfer_t s_receiveXfer[BUFFER_NUM];
static volatile uint32_t s_readIndex = 0U;
static const pdm_config_t pdmConfig = {
    .enableDoze        = false,
    .fifoWatermark     = DEMO_PDM_FIFO_WATERMARK,
    .qualityMode       = DEMO_PDM_QUALITY_MODE,
    .cicOverSampleRate = DEMO_PDM_CIC_OVERSAMPLE_RATE,
};

static const pdm_channel_config_t channelConfig = {
#if (defined(FSL_FEATURE_PDM_HAS_DC_OUT_CTRL) && (FSL_FEATURE_PDM_HAS_DC_OUT_CTRL))
    .outputCutOffFreq = kPDM_DcRemoverCutOff40Hz,
#else
    .cutOffFreq = kPDM_DcRemoverCutOff152Hz,
#endif
    .gain = kPDM_DfOutputGain1,
};

int Streamer_buff_addr_Push(uint8_t *buff_addr)
{
    if ((streamer_buff_addr.num_buffers <= 0) || (buff_addr == NULL))
    {
        return -1;
    }
    if (streamer_buff_addr.count == streamer_buff_addr.num_buffers)
    {
        return -1;
    }
    if (streamer_buff_addr.count == (streamer_buff_addr.num_buffers - 1))
    {
        rx_sem_take = true;
        (void)OSA_SemaphoreWait((osa_semaphore_handle_t)pcmHandle.semaphoreRX, osaWaitForever_c);
    }
    streamer_buff_addr.buff_addr[streamer_buff_addr.head] = buff_addr;
    streamer_buff_addr.head = (streamer_buff_addr.head + 1) % streamer_buff_addr.num_buffers;
    streamer_buff_addr.count++;
    return 0;
}

uint8_t *Streamer_buff_addr_Pull(void)
{
    uint8_t *buff_addr = NULL;
    if (0 == streamer_buff_addr.count)
    {
        return buff_addr;
    }
    buff_addr               = streamer_buff_addr.buff_addr[streamer_buff_addr.tail];
    streamer_buff_addr.tail = (streamer_buff_addr.tail + 1) % streamer_buff_addr.num_buffers;
    streamer_buff_addr.count--;
    return buff_addr;
}

/* pcmHandle.semaphoreTX/RX are OSA semaphore handles; OSA_SemaphorePost is
 * ISR-safe on the FreeRTOS backend. */
static void saiTxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    (void)OSA_SemaphorePost((osa_semaphore_handle_t)pcmHandle.semaphoreTX);
}

static void pdmRxCallback(PDM_Type *base, pdm_edma_handle_t *handle, status_t status, void *userData)
{
    rx_data_valid++;
    if (rx_sem_take == true)
    {
        rx_sem_take = false;
        (void)OSA_SemaphorePost((osa_semaphore_handle_t)pcmHandle.semaphoreRX);
    }
}

void streamer_pcm_init(void)
{
    edma_config_t dmaConfig;

    NVIC_SetPriority(DEMO_PDM_EVENT_IRQn, 5);
    NVIC_SetPriority(DEMO_SAI_IRQ, 5);

    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_EDMA, &dmaConfig);

    EDMA_CreateHandle(&pcmHandle.dmaTxHandle, DEMO_EDMA, DEMO_TX_CHANNEL);
    EDMA_CreateHandle(&pcmHandle.dmaRxHandle, DEMO_EDMA, DEMO_RX_CHANNEL);

    /* EDMA_CreateHandle() leaves the completion IRQs at priority 0, but their
     * callbacks post a FreeRTOS semaphore and so must run at a syscall-safe
     * priority (>= configMAX_SYSCALL_INTERRUPT_PRIORITY) or the first post
     * traps in vPortValidateInterruptPriority(). */
    NVIC_SetPriority(AUDIO_EDMA3_CH0_IRQn, 4); /* DEMO_TX_CHANNEL: SAI0 TX */
    NVIC_SetPriority(AUDIO_EDMA3_CH1_IRQn, 4); /* DEMO_RX_CHANNEL: MICFIL RX */
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_EDMA, DEMO_TX_CHANNEL, DEMO_SAI_TX_SOURCE);
    EDMA_SetChannelMux(DEMO_EDMA, DEMO_RX_CHANNEL, DEMO_SAI_RX_SOURCE);
#endif

    SAI_Init(DEMO_SAI);

    /* Enable SAI0 MCLK output so the WM8962 codec receives SYSCLK. */
    BOARD_MasterClockConfig();

    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &pcmHandle.saiTxHandle, saiTxCallback, (void *)&pcmHandle,
                                   &pcmHandle.dmaTxHandle);

    PDM_Init(DEMO_PDM, &pdmConfig);
    PDM_TransferCreateHandleEDMA(DEMO_PDM, &pcmHandle.pdmRxHandle, pdmRxCallback, (void *)&pcmHandle,
                                 &pcmHandle.dmaRxHandle);
    PDM_TransferInstallEDMATCDMemory(&pcmHandle.pdmRxHandle, s_edmaTcd, BUFFER_NUM);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &pcmHandle.pdmRxHandle, DEMO_PDM_ENABLE_CHANNEL_LEFT, &channelConfig);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &pcmHandle.pdmRxHandle, DEMO_PDM_ENABLE_CHANNEL_RIGHT, &channelConfig);
    if (PDM_SetSampleRateConfig(DEMO_PDM, DEMO_PDM_CLK_FREQ, DEMO_MIC_SAMPLE_RATE) != kStatus_Success)
    {
        PRINTF("\r\nPDM configure sample rate failed.\r\n");
    }
    /* Proven RT1170 reference order: SetSampleRateConfig -> EnableInterrupts -> Reset. */
    PDM_EnableInterrupts(DEMO_PDM, kPDM_ErrorInterruptEnable);
    PDM_Reset(DEMO_PDM);
    for (int ibuf = 0; ibuf < BUFFER_NUM; ibuf++)
    {
        s_receiveXfer[ibuf].data     = &s_buffer[ibuf * RECORD_BUFFER_SIZE];
        s_receiveXfer[ibuf].dataSize = RECORD_BUFFER_SIZE;
        int ibuf_next                = ibuf + 1;
        if (ibuf_next == BUFFER_NUM)
        {
            ibuf_next = 0;
        }
        s_receiveXfer[ibuf].linkTransfer = &s_receiveXfer[ibuf_next];
    }

    pcmHandle.isFirstRx      = 1;
    pcmHandle.isFirstTx      = 1;
    PDM_started              = 0;
    SAI_started              = 0;
    streamer_buff_addr.count = 0;
    streamer_buff_addr.head  = 0;
    streamer_buff_addr.tail  = 0;
    rx_data_valid            = 0;
    s_readIndex              = 0;
    rx_sem_take              = false;

    /* Arm the TCD ring but gate the DMA request: RX is released on the first
     * streamer_pcm_read() so it starts in lockstep with the TX consumer. */
    PDM_TransferReceiveEDMA(DEMO_PDM, &pcmHandle.pdmRxHandle, s_receiveXfer);
    PDM_EnableDMA(DEMO_PDM, false);
}

int streamer_pcm_tx_open(uint32_t num_buffers)
{
    return (OSA_SemaphoreCreateBinary((osa_semaphore_handle_t)pcmHandle.semaphoreTX) == KOSA_StatusSuccess) ? 0 : -1;
}

int streamer_pcm_rx_open(uint32_t num_buffers)
{
    return (OSA_SemaphoreCreateBinary((osa_semaphore_handle_t)pcmHandle.semaphoreRX) == KOSA_StatusSuccess) ? 0 : -1;
}

void streamer_pcm_tx_close(void)
{
    SAI_TransferTerminateSendEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle));
    (void)OSA_SemaphoreDestroy((osa_semaphore_handle_t)pcmHandle.semaphoreTX);
}

void streamer_pcm_rx_close(void)
{
    PDM_TransferTerminateReceiveEDMA(DEMO_PDM, &(pcmHandle.pdmRxHandle));
    PDM_Deinit(DEMO_PDM);
    (void)OSA_SemaphoreDestroy((osa_semaphore_handle_t)pcmHandle.semaphoreRX);
}

int streamer_pcm_write(uint8_t *data, uint32_t size)
{
    status_t st;

    /* Discard the first buffer for RX-TX synchronization. */
    if (pcmHandle.isFirstTx)
    {
        pcmHandle.isFirstTx = 0U;
        return 1;
    }

    /* Round down to the EDMA burst multiple (32 B for 16-bit, 64 B for 32-bit). */
    pcmHandle.saiTx.dataSize = size - (size % ((pcmHandle.bit_width == 16U) ? 32U : 64U));
    pcmHandle.saiTx.data     = data;

    /* TX buffers are in LLC-cached PSRAM but the EDMA master bypasses the LLC;
     * write each back before queuing or the DMA replays stale PSRAM (90 ms tick). */
    pcm_llc_writeback((uint32_t)(pcmHandle.saiTx.data), pcmHandle.saiTx.dataSize);

    /* Prime the SAI EDMA queue on the first write so the TX FIFO never underruns. */
    if (SAI_started == 0)
    {
        (void)SAI_TransferSendEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle), &(pcmHandle.saiTx));
        SAI_started = 1;
    }

    while ((st = SAI_TransferSendEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle), &(pcmHandle.saiTx))) ==
           kStatus_SAI_QueueFull)
    {
        if (OSA_SemaphoreWait((osa_semaphore_handle_t)pcmHandle.semaphoreTX, osaWaitForever_c) != KOSA_StatusSuccess)
        {
            return -1;
        }
    }
    if (st != kStatus_Success)
    {
        return -1;
    }
    return 0;
}

int streamer_pcm_read(uint8_t *data, uint32_t size)
{
    int ret = 1;

    if (size != RECORD_BUFFER_SIZE)
    {
        return -1;
    }

    if (pcmHandle.isFirstRx)
    {
        /* First cycle primes the downstream path; PDM DMA stays off. */
        pcmHandle.isFirstRx = 0;
    }
    else
    {
        /* Release PDM DMA on the first real read, in step with the TX consumer. */
        if (PDM_started == 0)
        {
            PDM_EnableDMA(DEMO_PDM, true);
            PDM_started = 1;
        }
        if (rx_data_valid > 0)
        {
            rx_data_valid--;
            memcpy(Streamer_buff_addr_Pull(), &s_buffer[s_readIndex], size);
            s_readIndex += size;
            if (s_readIndex >= BUFFER_SIZE)
            {
                s_readIndex -= BUFFER_SIZE;
            }
            ret = 0;
        }
    }

    if (Streamer_buff_addr_Push(data) != 0)
    {
        return -1;
    }
    return ret;
}

int streamer_pcm_setparams(
    uint32_t sample_rate, uint32_t bit_width, uint8_t num_channels, bool transfer, bool dummy_tx, int volume)
{
    sai_transceiver_t config;

    pcmHandle.sample_rate     = sample_rate;
    pcmHandle.bit_width       = bit_width;
    pcmHandle.num_channels    = num_channels;
    pcmHandle.dummy_tx_enable = dummy_tx;

    /* Configure SAI TX for the codec (transfer) or when dummy_tx needs BCLK/WS.
     * RT2660 capture is PDM, so there is no SAI-RX config. */
    if (transfer || dummy_tx)
    {
        SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
        config.bitClock.bclkSource = (sai_bclk_source_t)DEMO_SAI_CLOCK_SOURCE;
        config.masterSlave         = DEMO_SAI_MASTER_SLAVE;
        SAI_TransferTerminateSendEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle));
        SAI_TransferTxSetConfigEDMA(DEMO_SAI, &(pcmHandle.saiTxHandle), &config);
        SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_MIC_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                              (pcmHandle.num_channels == 1U) ? 2U : pcmHandle.num_channels);
        SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
        BOARD_MASTER_CLOCK_CONFIG(); /* no-op: MCLK already driven by BOARD_InitHardware */
        /* Re-arm the first-buffer discard so a second run still primes correctly. */
        pcmHandle.isFirstTx = 1U;
    }

    /* Push the current sample-rate/format/volume to the WM8962. */
    CODEC_Init(&codecHandle, &boardCodecConfig);
    CODEC_SetFormat(&codecHandle, DEMO_AUDIO_MASTER_CLOCK, DEMO_MIC_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH);
    CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight, DEMO_VOLUME);
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
    int channel;

    switch (pcmHandle.num_channels)
    {
        case 0:
            return 0;
        case 1:
            channel = kCODEC_PlayChannelHeadphoneLeft;
            break;
        case 2:
            channel = kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight;
            break;
        default:
            channel = ALL_CHANNELS;
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
