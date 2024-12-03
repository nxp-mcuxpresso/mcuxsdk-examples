/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_dmamux.h"
#include "fsl_flexio_i2s_dma.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OVER_SAMPLE_RATE (384)
#define BUFFER_SIZE      (256)
#if defined(__GNUC__) /* GNU Compiler */
#ifndef __ALIGN_END
#define __ALIGN_END __attribute__((aligned(4)))
#endif
#ifndef __ALIGN_BEGIN
#define __ALIGN_BEGIN
#endif
#else
#ifndef __ALIGN_END
#define __ALIGN_END
#endif
#ifndef __ALIGN_BEGIN
#if defined(__CC_ARM) || defined(_ARMCC_VERSION) /* ARM Compiler */
#define __ALIGN_BEGIN __attribute__((aligned(4)))
#elif defined(__ICCARM__) /* IAR Compiler */
#define __ALIGN_BEGIN
#endif
#endif
#endif
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
#define DEMO_AUDIO_BIT_WIDTH (kSAI_WordWidth16bits)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexio_i2s_dma_handle_t txHandle = {0};
flexio_i2s_dma_handle_t rxHandle = {0};
dma_handle_t txDmaHandle         = {0};
dma_handle_t rxDmaHandle         = {0};
#if defined(__ICCARM__)
#pragma data_alignment = 4
#endif
static uint8_t __ALIGN_BEGIN txBuff[BUFFER_SIZE] __ALIGN_END;
#if defined(__ICCARM__)
#pragma data_alignment = 4
#endif
static uint8_t __ALIGN_BEGIN rxBuff[BUFFER_SIZE] __ALIGN_END;
codec_handle_t codecHandle = {0};
extern codec_config_t boardCodecConfig;
static volatile bool istxFinished = false;
static volatile bool isrxFinished = false;
FLEXIO_I2S_Type s_base;
#if (defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)) || \
    (defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER))
sai_master_clock_t mclkConfig = {
#if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    .mclkOutputEnable = true,
#if !(defined(FSL_FEATURE_SAI_HAS_NO_MCR_MICS) && (FSL_FEATURE_SAI_HAS_NO_MCR_MICS))
    .mclkSource = kSAI_MclkSourceSysclk,
#endif
#endif
};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
static void txCallback(FLEXIO_I2S_Type *i2sBase, flexio_i2s_dma_handle_t *handle, status_t status, void *userData)
{
    istxFinished = true;
}

static void rxCallback(FLEXIO_I2S_Type *i2sBase, flexio_i2s_dma_handle_t *handle, status_t status, void *userData)
{
    isrxFinished = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexio_i2s_config_t config;
    flexio_i2s_format_t format;
    flexio_i2s_transfer_t xferTx, xferRx;
    uint32_t i      = 0;
    status_t status = kStatus_Success;
    sai_transceiver_t saiConfig;

    BOARD_InitHardware();
    BOARD_Codec_I2C_Init();

    PRINTF("FLEXIO I2S DMA example started!\n\r");

    /* Set flexio i2s pin, shifter and timer */
    s_base.bclkPinIndex   = BCLK_PIN;
    s_base.fsPinIndex     = FRAME_SYNC_PIN;
    s_base.txPinIndex     = TX_DATA_PIN;
    s_base.rxPinIndex     = RX_DATA_PIN;
    s_base.txShifterIndex = 0;
    s_base.rxShifterIndex = 1;
    s_base.bclkTimerIndex = 0;
    s_base.fsTimerIndex   = 1;
    s_base.flexioBase     = DEMO_FLEXIO_BASE;

#if defined(DEMO_CODEC_WM8960)
    /* SAI init */
    SAI_Init(DEMO_SAI);

    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&saiConfig, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    SAI_TxSetConfig(DEMO_SAI, &saiConfig);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
#if (defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)) || \
    (defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER))
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    mclkConfig.mclkHz          = DEMO_AUDIO_MASTER_CLOCK;
    mclkConfig.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;
#endif
    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
#endif

    SAI_TxEnable(DEMO_SAI, true);
#endif
#endif
    /* Create DMA handle */
    DMA_Init(EXAMPLE_DMA);
    DMA_CreateHandle(&txDmaHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
    DMA_CreateHandle(&rxDmaHandle, EXAMPLE_DMA, EXAMPLE_RX_CHANNEL);

    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, EXAMPLE_TX_CHANNEL, EXAMPLE_TX_DMA_SOURCE);
    DMAMUX_EnableChannel(DMAMUX0, EXAMPLE_TX_CHANNEL);
    DMAMUX_SetSource(DMAMUX0, EXAMPLE_RX_CHANNEL, EXAMPLE_RX_DMA_SOURCE);
    DMAMUX_EnableChannel(DMAMUX0, EXAMPLE_RX_CHANNEL);

    /* Init SAI module */
    /*
     * config.enableI2S = true;
     */
    FLEXIO_I2S_GetDefaultConfig(&config);
    FLEXIO_I2S_Init(&s_base, &config);

    /* Configure the audio format */
    format.bitWidth      = DEMO_AUDIO_BIT_WIDTH;
    format.sampleRate_Hz = DEMO_AUDIO_SAMPLE_RATE;

    /* Use default setting to init codec */
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    if (CODEC_SetFormat(&codecHandle, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH) !=
        kStatus_Success)
    {
        assert(false);
    }

    FLEXIO_I2S_TransferTxCreateHandleDMA(&s_base, &txHandle, txCallback, NULL, &txDmaHandle);
    FLEXIO_I2S_TransferRxCreateHandleDMA(&s_base, &rxHandle, rxCallback, NULL, &rxDmaHandle);

    FLEXIO_I2S_TransferSetFormatDMA(&s_base, &txHandle, &format, DEMO_FLEXIO_CLK_FREQ);
    FLEXIO_I2S_TransferSetFormatDMA(&s_base, &rxHandle, &format, DEMO_FLEXIO_CLK_FREQ);

    /*  xfer structure */
    xferTx.data     = txBuff;
    xferTx.dataSize = BUFFER_SIZE;
    xferRx.data     = rxBuff;
    xferRx.dataSize = BUFFER_SIZE;
    /* Add a transfer to trigger bit clock */
    FLEXIO_I2S_TransferSendDMA(&s_base, &txHandle, &xferTx);
    /* First record a piece of data */
    for (i = 0; i < 5000; i++)
    {
        isrxFinished = false;
        /* Fill the queue to full */
        status = kStatus_Success;
        while (status != kStatus_FLEXIO_I2S_QueueFull)
        {
            status = FLEXIO_I2S_TransferReceiveDMA(&s_base, &rxHandle, &xferRx);
        }
        /* Wait for data record finished */
        while (isrxFinished != true)
        {
        }
        /* Copy data from RX buffer to tx buffer */
        memcpy(txBuff, rxBuff, BUFFER_SIZE);
        FLEXIO_I2S_TransferSendDMA(&s_base, &txHandle, &xferTx);
    }

    if (CODEC_Deinit(&codecHandle) != kStatus_Success)
    {
        assert(false);
    }
    PRINTF("\n\r FLEXIO I2S DMA example finished!\n\r ");

    while (1)
    {
    }
}
