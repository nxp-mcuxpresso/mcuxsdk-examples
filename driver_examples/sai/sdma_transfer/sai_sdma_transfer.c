/*
 * Copyright 2017 NXP
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
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OVER_SAMPLE_RATE (384U)
/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate16KHz)
/* demo audio master clock */
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
#define DEMO_AUDIO_MASTER_CLOCK OVER_SAMPLE_RATE *DEMO_AUDIO_SAMPLE_RATE
#else
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#endif
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (2U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth16bits
#ifndef DEMO_CODEC_INIT_DELAY_MS
#define DEMO_CODEC_INIT_DELAY_MS (1000U)
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void callback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(sai_sdma_handle_t txHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t dmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t context, 4);
volatile bool isFinished = false;
extern codec_config_t boardCodecConfig;
codec_handle_t codecHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void callback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData)
{
    isFinished = true;
}

void DelayMS(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        SDK_DelayAtLeastUs(1000, SystemCoreClock);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    sai_transfer_t xfer;
    sdma_config_t dmaConfig = {0};
    uint32_t temp           = 0;
    sai_transceiver_t saiConfig;
    status_t error = kStatus_Success;

    BOARD_InitHardware();

    PRINTF("SAI SDMA example started!\n\r");
    /* Create SDMA handle */
    SDMA_GetDefaultConfig(&dmaConfig);
#ifdef SDMA_FREQ_EQUALS_ARM
    dmaConfig.ratio = kSDMA_ARMClockFreq;
#endif

    SDMA_Init(EXAMPLE_DMA, &dmaConfig);
    SDMA_CreateHandle(&dmaHandle, EXAMPLE_DMA, EXAMPLE_CHANNEL, &context);
    SDMA_SetChannelPriority(EXAMPLE_DMA, EXAMPLE_CHANNEL, 2);

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleSDMA(DEMO_SAI, &txHandle, callback, NULL, &dmaHandle, SAI_TX_SOURCE);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&saiConfig, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    saiConfig.bitClock.bclkSource = (sai_bclk_source_t)DEMO_SAI_CLOCK_SOURCE;
    saiConfig.masterSlave         = DEMO_SAI_MASTER_SLAVE;
    SAI_TransferTxSetConfigSDMA(DEMO_SAI, &txHandle, &saiConfig);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

    /* Use default setting to init codec */
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    /* delay for codec output stable */
    DelayMS(DEMO_CODEC_INIT_DELAY_MS);

    /* Enable SAI interrupt to clear the FIFO error */
    EnableIRQ(DEMO_IRQn);
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);

    /*  xfer structure */
    temp          = (uint32_t)music;
    xfer.data     = (uint8_t *)temp;
    xfer.dataSize = MUSIC_LEN;
    SAI_TransferSendSDMA(DEMO_SAI, &txHandle, &xfer);

    /* Wait until finished */
    while (isFinished != true)
    {
    }

    /* Once transfer finish, disable SAI instance. */
    SAI_TransferAbortSendSDMA(DEMO_SAI, &txHandle);
    SAI_Deinit(DEMO_SAI);

    error = CODEC_Deinit(&codecHandle);
    if ((error != kStatus_CODEC_NotSupport) && (error != kStatus_Success))
    {
        assert(false);
    }

    PRINTF("\n\r SAI SDMA example finished!\n\r ");
    while (1)
    {
    }
}

void SAI_UserIRQHandler(void)
{
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    SDK_ISR_EXIT_BARRIER;
}
