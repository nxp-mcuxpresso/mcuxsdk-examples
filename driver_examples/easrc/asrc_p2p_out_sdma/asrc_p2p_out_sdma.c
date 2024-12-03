/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_sai_sdma.h"
#include "music.h"
#include "fsl_codec_common.h"
#include "fsl_asrc_sdma.h"
#include "fsl_asrc.h"
#include "fsl_sdma_script.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* convert buffer size, please note that the maximum size  of once transfer that SDMA can support is 64k */
#define DEMO_AUDIO_BUFFER_SIZE (16000U)
#define DEMO_AUDIO_SAMPLES     (MUSIC_LEN / 2U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void asrcConvertAudio(void *in, uint32_t dataSize);
static void startSai(bool start);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool isConvertFinished = false;
extern codec_config_t boardCodecConfig;
codec_handle_t codecHandle;
/*! @brief ASRC sdma handle */
AT_NONCACHEABLE_SECTION_ALIGN(static asrc_sdma_handle_t s_asrcHandle, 4U);
AT_NONCACHEABLE_SECTION_ALIGN(static sdma_handle_t s_asrcInSDMAHandle, 4U);
AT_NONCACHEABLE_SECTION_ALIGN(static sdma_handle_t s_asrcOutSDMAHandle, 4U);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t s_asrcInSDMAcontext, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t s_asrcOutSDMAcontext, 4);

/*! @brief ASRC context config */
static asrc_context_config_t s_asrcContextConfig;
static asrc_p2p_sdma_config_t s_asrcSDMAConfig = {
    .eventSource      = DEMO_SAI_TX_SOURCE,
    .channel          = 0U,
    .enableContinuous = true,
    .startPeripheral  = startSai,
};
const short g_sdma_multi_fifo_script[] = FSL_SDMA_MULTI_FIFO_SCRIPT;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void asrcInCallback(ASRC_Type *base, asrc_sdma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_ASRCQueueIdle)
    {
        isConvertFinished = true;
    }
}

static void startSai(bool start)
{
    if (start)
    {
        SAI_TxEnable(DEMO_SAI, true);
    }
    else
    {
        SAI_TxEnable(DEMO_SAI, false);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t *temp              = (uint8_t *)music;
    sdma_config_t dmaConfig    = {0};
    s_asrcSDMAConfig.watermark = FSL_FEATURE_SAI_FIFO_COUNTn(DEMO_SAI) / 2U;
    sai_transceiver_t config;

    BOARD_InitHardware();

    PRINTF("ASRC peripheral to peripheral SDMA example.\n\r");

    /* Create SDMA handle */
    SDMA_GetDefaultConfig(&dmaConfig);
#ifdef SDMA_FREQ_EQUALS_ARM
    dmaConfig.ratio = kSDMA_ARMClockFreq;
#endif

    SDMA_Init(DEMO_DMA, &dmaConfig);

    SDMA_LoadScript(DEMO_DMA, FSL_SDMA_SCRIPT_CODE_START_ADDR, (void *)g_sdma_multi_fifo_script,
                    FSL_SDMA_SCRIPT_CODE_SIZE);

    /* SAI init */
    SAI_Init(DEMO_SAI);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    config.bitClock.bclkSource = (sai_bclk_source_t)DEMO_SAI_CLOCK_SOURCE;
    SAI_TxSetConfig(DEMO_SAI, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_ASRC_OUT_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

    /* Enable DMA enable bit */
    SAI_TxEnableDMA(DEMO_SAI, kSAI_FIFORequestDMAEnable, true);

    /* create ASRC sdma handle */
    SDMA_CreateHandle(&s_asrcInSDMAHandle, DEMO_DMA, DEMO_ASRC_IN_SDMA_CHANNEL, &s_asrcInSDMAcontext);
    SDMA_SetChannelPriority(DEMO_DMA, DEMO_ASRC_IN_SDMA_CHANNEL, 4);
    SDMA_CreateHandle(&s_asrcOutSDMAHandle, DEMO_DMA, DEMO_ASRC_OUT_SDMA_CHANNEL, &s_asrcOutSDMAcontext);
    SDMA_SetChannelPriority(DEMO_DMA, DEMO_ASRC_OUT_SDMA_CHANNEL, 3);

    ASRC_Init(DEMO_ASRC);
    ASRC_GetContextDefaultConfig(&s_asrcContextConfig, DEMO_AUDIO_DATA_CHANNEL, DEMO_ASRC_IN_SAMPLE_RATE,
                                 DEMO_ASRC_OUT_SAMPLE_RATE);
    ASRC_TransferInCreateHandleSDMA(DEMO_ASRC, &s_asrcHandle, asrcInCallback, &s_asrcInSDMAHandle,
                                    DEMO_ASRC_IN_SDMA_SOURCE, DEMO_ASRC_CONTEXT, NULL, NULL);
    ASRC_TransferOutCreateHandleSDMA(DEMO_ASRC, &s_asrcHandle, NULL, &s_asrcOutSDMAHandle, DEMO_ASRC_OUT_SDMA_SOURCE,
                                     DEMO_ASRC_CONTEXT, &s_asrcSDMAConfig, NULL);
    if (ASRC_TransferSetContextConfigSDMA(DEMO_ASRC, &s_asrcHandle, &s_asrcContextConfig) != kStatus_Success)
    {
        PRINTF("ASRC context configure failed, please check.\r\n");
        return -1;
    }

    /* Use default setting to init codec */
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }

    PRINTF("\r\nPlayback converted audio data\r\n");
    PRINTF("    sample rate : %d\r\n", DEMO_ASRC_OUT_SAMPLE_RATE);
    PRINTF("    channel number: %d\r\n", DEMO_AUDIO_DATA_CHANNEL);
    PRINTF("    frequency: 1kHZ.\r\n\r\n");

    asrcConvertAudio(temp, MUSIC_LEN);

    PRINTF("ASRC peripheral to peripheral SDMA example finished.\n\r ");

    ASRC_Deinit(DEMO_ASRC);

    while (1)
    {
    }
}

static void asrcConvertAudio(void *in, uint32_t dataSize)
{
    asrc_transfer_t xfer;

    xfer.inDataAddr  = in;
    xfer.inDataSize  = dataSize;
    xfer.outDataAddr = (uint32_t *)SAI_TxGetDataRegisterAddress(DEMO_SAI, DEMO_SAI_CHANNEL_NUM);
    xfer.outDataSize =
        ASRC_GetContextOutSampleSize(DEMO_ASRC_IN_SAMPLE_RATE, dataSize, 2U, DEMO_ASRC_OUT_SAMPLE_RATE, 2U);
    ASRC_TransferSDMA(DEMO_ASRC, &s_asrcHandle, &xfer);

    /* Wait until finished */
    while (!isConvertFinished)
    {
    }
}

void SAI_UserIRQHandler(void)
{
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    SDK_ISR_EXIT_BARRIER;
}
