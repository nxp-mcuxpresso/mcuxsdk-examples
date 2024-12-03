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
#include "fsl_asrc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* demo audio bit width */
#define DEMO_AUDIO_BUFFER_SAMPLES (24000)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void saiCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData);
static void saiPlayAudio(uint8_t *data, uint32_t dataSize);
/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(sai_sdma_handle_t txHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t dmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t s_saiContext, 4);
volatile uint8_t isSaiFinishedCount = false;
extern codec_config_t boardCodecConfig;
codec_handle_t codecHandle;

AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_asrcOutBuffer[DEMO_AUDIO_BUFFER_SAMPLES * 2U], 4);
static asrc_context_config_t s_asrcContextConfig;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void saiCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData)
{
    isSaiFinishedCount++;
}

/*!
 * @brief Main function
 */
int main(void)
{
    asrc_transfer_t asrcTransfer;
    sai_transceiver_t config;

    BOARD_InitHardware();

    PRINTF("ASRC memory to memory polling example.\n\r");

    memset(s_asrcOutBuffer, 0U, sizeof(s_asrcOutBuffer));

    /* peripheral initialization */
    sdma_config_t dmaConfig = {0};

    /* Create SDMA handle */
    SDMA_GetDefaultConfig(&dmaConfig);
#ifdef SDMA_FREQ_EQUALS_ARM
    dmaConfig.ratio = kSDMA_ARMClockFreq;
#endif
    SDMA_Init(DEMO_DMA, &dmaConfig);

    ASRC_Init(DEMO_ASRC);
    ASRC_GetContextDefaultConfig(&s_asrcContextConfig, DEMO_AUDIO_DATA_CHANNEL, DEMO_ASRC_IN_SAMPLE_RATE,
                                 DEMO_ASRC_OUT_SAMPLE_RATE);
    if (ASRC_SetContextConfig(DEMO_ASRC, DEMO_ASRC_CONTEXT, &s_asrcContextConfig) != kStatus_Success)
    {
        PRINTF("ASRC context configure failed, please check.\r\n");
        return -1;
    }

    /* create sai sdma handle */
    SDMA_CreateHandle(&dmaHandle, DEMO_DMA, DEMO_SAI_SDMA_CHANNEL, &s_saiContext);
    SDMA_SetChannelPriority(DEMO_DMA, DEMO_SAI_SDMA_CHANNEL, 2);

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleSDMA(DEMO_SAI, &txHandle, saiCallback, NULL, &dmaHandle, DEMO_SAI_TX_SOURCE);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    config.bitClock.bclkSource = (sai_bclk_source_t)DEMO_SAI_CLOCK_SOURCE;
    SAI_TransferTxSetConfigSDMA(DEMO_SAI, &txHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_ASRC_IN_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

    /* Enable SAI interrupt to clear the FIFO error */
    EnableIRQ(DEMO_IRQn);
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);

    /* Use default setting to init codec */
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }

    PRINTF("Playback raw audio data\r\n");
    PRINTF("    sample rate : %d\r\n", DEMO_ASRC_IN_SAMPLE_RATE);
    PRINTF("    channel number: %d\r\n", DEMO_AUDIO_DATA_CHANNEL);
    PRINTF("    frequency: 215HZ.\r\n\r\n");

    /*  xfer structure */
    saiPlayAudio((uint8_t *)music, MUSIC_LEN);

    asrcTransfer.inDataAddr  = (uint32_t *)music;
    asrcTransfer.inDataSize  = MUSIC_LEN;
    asrcTransfer.outDataAddr = s_asrcOutBuffer;
    asrcTransfer.outDataSize =
        ASRC_GetContextOutSampleSize(DEMO_ASRC_IN_SAMPLE_RATE, MUSIC_LEN, 2U, DEMO_ASRC_OUT_SAMPLE_RATE, 2U);
    ASRC_TransferBlocking(DEMO_ASRC, DEMO_ASRC_CONTEXT, &asrcTransfer);

    /* reconfigure the sai clock */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_ASRC_OUT_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    PRINTF("Playback converted audio data\r\n");
    PRINTF("    sample rate : %d\r\n", DEMO_ASRC_OUT_SAMPLE_RATE);
    PRINTF("    channel number: %d\r\n", DEMO_AUDIO_DATA_CHANNEL);
    PRINTF("    frequency: 215HZ.\r\n\r\n");

    saiPlayAudio((uint8_t *)s_asrcOutBuffer,
                 ASRC_GetContextOutSampleSize(DEMO_ASRC_IN_SAMPLE_RATE, MUSIC_LEN, 2U, DEMO_ASRC_OUT_SAMPLE_RATE, 2U));

    /* Once transfer all finish, de-initializes the SAI peripheral */
    SAI_Deinit(DEMO_SAI);
    PRINTF("ASRC memory to memory polling example finished.\n\r ");
    while (1)
    {
    }
}

static void saiPlayAudio(uint8_t *data, uint32_t dataSize)
{
    sai_transfer_t xfer;
    uint32_t size = dataSize;
    uint8_t *addr = data;
    uint8_t count = 0U;

    while (size)
    {
        /*  xfer structure */
        xfer.data     = addr;
        xfer.dataSize = size > 64000 ? 64000 : size;
        if (SAI_TransferSendSDMA(DEMO_SAI, &txHandle, &xfer) == kStatus_Success)
        {
            size -= xfer.dataSize;
            addr += xfer.dataSize;
            count++;
        }
    }

    while (isSaiFinishedCount != count)
    {
    }
    isSaiFinishedCount = 0U;

    /* Once transfer finish, disable SAI instance and clear all the internal information. */
    SAI_TransferTerminateSendSDMA(DEMO_SAI, &txHandle);
}

void SAI_UserIRQHandler(void)
{
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    SDK_ISR_EXIT_BARRIER;
}
