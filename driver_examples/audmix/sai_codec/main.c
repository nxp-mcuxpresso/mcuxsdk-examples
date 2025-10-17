/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_audmix.h"
#include "fsl_sai.h"
#include "fsl_sai_edma.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "music.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_SIZE   (1024U)  /* Each buffer is 1/4 of the total music data */
#define BUFFER_NUMBER (4U)     /* 4 buffers to hold the entire 4096 bytes of music data */
#define PLAY_COUNT    (10U)    /* Play the music data 10 times */

#ifndef DEMO_DMAMUX
#define DEMO_DMAMUX DMAMUX0
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_InitSAI(void);
static void DEMO_InitAUDMIX(void);
static void DEMO_TxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
static void DEMO_InitEDMA(void);
static void DEMO_StartSAITDMTransfer(void);
static void DEMO_ProcessTDMChannels(uint8_t *srcBuf, uint8_t *destBuf, uint32_t size);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION(sai_edma_handle_t txHandle1);
AT_NONCACHEABLE_SECTION(sai_edma_handle_t txHandle2);
AT_NONCACHEABLE_SECTION(sai_edma_handle_t rxHandle);
AT_NONCACHEABLE_SECTION(edma_handle_t dmaTxHandle1);
AT_NONCACHEABLE_SECTION(edma_handle_t dmaTxHandle2);
AT_NONCACHEABLE_SECTION(edma_handle_t dmaRxHandle);

AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_buffer[BUFFER_SIZE], 4);

static volatile uint32_t s_transferCount = 0;
static volatile bool s_isFinished = false;

/* SAI transfer configurations */
sai_transfer_t saiTxTransfer1[BUFFER_NUMBER];
sai_transfer_t saiTxTransfer2[BUFFER_NUMBER];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("AUDMIX Example started!\r\n");

    /* Initialize EDMA */
    DEMO_InitEDMA();

    /* Initialize SAI peripherals */
    DEMO_InitSAI();

    /* Initialize AUDMIX peripheral */
    DEMO_InitAUDMIX();

    /* Start SAI TDM transfers */
    DEMO_StartSAITDMTransfer();

    /* Check if TDM streams are synchronized */
    if (AUDMIX_IsFrameRateMatched(DEMO_AUDMIX) && AUDMIX_IsClockFrequencyMatched(DEMO_AUDMIX))
    {
        PRINTF("TDM streams are synchronized, enabling mixed output.\r\n");
        /* Configure AUDMIX for mixed output */
        AUDMIX_SetOutputSource(DEMO_AUDMIX, kAUDMIX_OutputMixed);

        /* Setup attenuation for TDM1 */
        audmix_attenuation_config_t attConfig;
        AUDMIX_GetDefaultAttenuationConfig(&attConfig);
        attConfig.attenuationEnable = true;
        attConfig.attenuationDirection = kAUDMIX_AttenuationDirectionDown;
        AUDMIX_SetAttenuationConfig(DEMO_AUDMIX, 0, &attConfig);

        /* Wait for a while to let attenuation reach target */
        SDK_DelayAtLeastUs(1000, SystemCoreClock);

        /* Change attenuation direction to up */
        AUDMIX_SetAttenuationDirection(DEMO_AUDMIX, 0, kAUDMIX_AttenuationDirectionUp);

        /* Wait for a while to let attenuation reach target */
        SDK_DelayAtLeastUs(1000, SystemCoreClock);

        /* Disable attenuation */
        AUDMIX_EnableAttenuation(DEMO_AUDMIX, 0, false);
    }
    else
    {
        PRINTF("TDM streams are not synchronized, using TDM1 output only.\r\n");
        AUDMIX_SetOutputSource(DEMO_AUDMIX, kAUDMIX_OutputFromTDM1);
    }

    /* Wait for transfers to complete */
    while (!s_isFinished)
    {
    }

    PRINTF("AUDMIX Example completed!\r\n");

    /* Cleanup */
    SAI_TransferAbortSendEDMA(DEMO_SAI1, &txHandle1);
    SAI_TransferAbortSendEDMA(DEMO_SAI2, &txHandle2);
    SAI_TransferAbortSendEDMA(DEMO_SAI3, &txHandle2);
    SAI_Deinit(DEMO_SAI1);
    SAI_Deinit(DEMO_SAI2);
    SAI_Deinit(DEMO_SAI3);
    AUDMIX_Deinit(DEMO_AUDMIX);

    while (1)
    {
    }
}

/*!
 * @brief Initialize the EDMA
 */
static void DEMO_InitEDMA(void)
{
    edma_config_t dmaConfig = {0};

    /* Create EDMA handle */
    EDMA_GetDefaultConfig(&dmaConfig);
#if defined(BOARD_GetEDMAConfig)
    BOARD_GetEDMAConfig(dmaConfig);
#endif
    EDMA_Init(DEMO_DMA, &dmaConfig);

    /* Create EDMA handles for SAI1 and SAI2 */
    EDMA_CreateHandle(&dmaTxHandle1, DEMO_DMA, DEMO_EDMA_CHANNEL);
    EDMA_CreateHandle(&dmaTxHandle2, DEMO_DMA, DEMO_EDMA_CHANNEL + 1);
    EDMA_CreateHandle(&dmaRxHandle, DEMO_DMA, DEMO_EDMA_CHANNEL + 2);

#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
#if defined(DEMO_SAI1_TX_SOURCE)
    EDMA_SetChannelMux(DEMO_DMA, DEMO_EDMA_CHANNEL, DEMO_SAI1_TX_SOURCE);
#endif
#if defined(DEMO_SAI2_TX_SOURCE)
    EDMA_SetChannelMux(DEMO_DMA, DEMO_EDMA_CHANNEL + 1, DEMO_SAI2_TX_SOURCE);
#endif
#if defined(DEMO_SAI2_TX_SOURCE)
    EDMA_SetChannelMux(DEMO_DMA, DEMO_EDMA_CHANNEL + 2, DEMO_SAI2_TX_SOURCE);
#endif
#endif

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
    DMAMUX_Init(DEMO_DMAMUX);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_EDMA_CHANNEL, DEMO_SAI1_TX_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_EDMA_CHANNEL);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_EDMA_CHANNEL + 1, DEMO_SAI2_TX_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_EDMA_CHANNEL + 1);
#endif
}

/*!
 * @brief Initialize the SAI peripherals
 */
static void DEMO_InitSAI(void)
{
    sai_transceiver_t saiTxConfig = {0};
    sai_transceiver_t saiRxConfig = {0};
    uint32_t sourceClock = DEMO_SAI_CLK_FREQ;

    /* Initialize SAI1 (TDM1 input) */
    SAI_Init(DEMO_SAI1);

    /* Initialize SAI2 (TDM2 input) */
    SAI_Init(DEMO_SAI2);

    /* Initialize SAI3 (AUDMIX output) */
    SAI_Init(DEMO_SAI3);

    /* Create EDMA SAI handle */
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI1, &txHandle1, DEMO_TxCallback, NULL, &dmaTxHandle1);
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI2, &txHandle2, DEMO_TxCallback, NULL, &dmaTxHandle2);
    SAI_TransferRxCreateHandleEDMA(DEMO_SAI3, &rxHandle, DEMO_TxCallback, NULL, &dmaRxHandle);

    /* Configure SAI1 for TDM mode with 8 channels */
    SAI_GetTDMConfig(&saiTxConfig, kSAI_FrameSyncLenOneBitClk, DEMO_SAI_BITWIDTH, DEMO_SAI_CHANNEL_NUM,
                     kSAI_Channel0Mask);

    /* Set master/slave configuration */
    saiTxConfig.masterSlave = kSAI_Master;
    saiTxConfig.syncMode = kSAI_ModeAsync;

    /* Configure bit clock */
    saiTxConfig.bitClock.bclkSource = kSAI_BclkSourceMclkDiv;
    saiTxConfig.bitClock.bclkPolarity = kSAI_PolarityActiveHigh;

    /* Configure frame sync */
    saiTxConfig.frameSync.frameSyncPolarity = kSAI_PolarityActiveHigh;
    saiTxConfig.frameSync.frameSyncEarly = true;

    /* Apply configuration to both SAI */
    SAI_TransferTxSetConfigEDMA(DEMO_SAI1, &txHandle1, &saiTxConfig);
    SAI_TransferTxSetConfigEDMA(DEMO_SAI2, &txHandle2, &saiTxConfig);

    /* Set bit clock rate for both SAI */
    SAI_TxSetBitClockRate(DEMO_SAI1, sourceClock, DEMO_SAI_SAMPLE_RATE, DEMO_SAI_BITWIDTH, DEMO_SAI_CHANNEL_NUM);
    SAI_TxSetBitClockRate(DEMO_SAI2, sourceClock, DEMO_SAI_SAMPLE_RATE, DEMO_SAI_BITWIDTH, DEMO_SAI_CHANNEL_NUM);

    /* Configure SAI3 for receiving the mixed output */
    memset(&saiRxConfig, 0, sizeof(saiRxConfig));
    SAI_GetTDMConfig(&saiRxConfig, kSAI_FrameSyncLenPerWordWidth, DEMO_SAI_BITWIDTH, DEMO_SAI_CHANNEL_NUM,
                     kSAI_Channel1Mask);
    saiRxConfig.masterSlave = kSAI_Slave;
    saiRxConfig.syncMode = kSAI_ModeSync;
    saiRxConfig.bitClock.bclkPolarity = kSAI_PolarityActiveHigh;
    saiRxConfig.frameSync.frameSyncPolarity = kSAI_PolarityActiveHigh;
    saiRxConfig.frameSync.frameSyncEarly = true;
    SAI_TransferRxSetConfigEDMA(DEMO_SAI3, &rxHandle, &saiRxConfig);

    /* Enable SAI transmitters and receivers */
    SAI_TxEnable(DEMO_SAI1, true);
    SAI_TxEnable(DEMO_SAI2, true);
    SAI_RxEnable(DEMO_SAI3, true);
}

/*!
 * @brief Initialize the AUDMIX peripheral
 */
static void DEMO_InitAUDMIX(void)
{
    audmix_config_t audmixConfig;
    audmix_attenuation_config_t attenuationConfig;

    /* Initialize AUDMIX */
    AUDMIX_Init(DEMO_AUDMIX);

    /* Configure AUDMIX */
    AUDMIX_GetDefaultConfig(&audmixConfig);
    audmixConfig.outputWidth = kAUDMIX_OutputWidth24Bit;
    audmixConfig.outputClockPolarity = kAUDMIX_OutputClockPolarityPositiveEdge;
    audmixConfig.mixClockSource = kAUDMIX_MixClockFromTDM1;
    audmixConfig.outputSource = kAUDMIX_OutputDisabled; /* Start with output disabled */
    AUDMIX_SetConfig(DEMO_AUDMIX, &audmixConfig);

    AUDMIX_GetDefaultAttenuationConfig(&attenuationConfig);
    AUDMIX_SetAttenuationConfig(DEMO_AUDMIX, 0, &attenuationConfig);

    /* Wait for a while to ensure TDM streams are running */
    SDK_DelayAtLeastUs(1000, SystemCoreClock);

    /* Check if TDM streams are synchronized */
    PRINTF("TDM clock match status: %s\r\n", AUDMIX_IsClockFrequencyMatched(DEMO_AUDMIX) ? "Matched" : "Mismatched");
    PRINTF("TDM frame rate match status: %s\r\n", AUDMIX_IsFrameRateMatched(DEMO_AUDMIX) ? "Matched" : "Mismatched");
}

/*!
 * @brief SAI EDMA callback
 */
static void DEMO_TxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxError == status)
    {
        /* Handle the error */
    }
    else
    {
        s_transferCount++;

        if (s_transferCount >= PLAY_COUNT)
        {
            s_isFinished = true;
        }
    }
}

/*!
 * @brief Start SAI TDM transfers
 */
static void DEMO_StartSAITDMTransfer(void)
{
    uint32_t i;

    PRINTF("Starting TDM audio playback for %d iterations...\r\n", PLAY_COUNT);

    /* Process the music data to create reversed channel order for SAI2 */
    DEMO_ProcessTDMChannels((uint8_t *)music, s_buffer, BUFFER_SIZE);

    /* Set up the transfers for all 4 segments of the music data */
    for (i = 0; i < BUFFER_NUMBER; i++)
    {
        uint32_t musicOffset = i * BUFFER_SIZE;

        /* Schedule SAI1 transfers (normal channel order) */
        saiTxTransfer1[i].data = (uint8_t *)&music[musicOffset];
        saiTxTransfer1[i].dataSize = BUFFER_SIZE;

        /* Schedule SAI2 transfers (reversed channel order) */
        /* For the first buffer, we already processed it above */
        if (i == 0)
        {
            saiTxTransfer2[i].data = s_buffer;
        }
        else
        {
            /* Process this segment of the music data for reversed channel order */
            DEMO_ProcessTDMChannels((uint8_t *)&music[musicOffset], s_buffer, BUFFER_SIZE);
            saiTxTransfer2[i].data = s_buffer;
        }
        saiTxTransfer2[i].dataSize = BUFFER_SIZE;
    }
    /* Submit the transfers */
    SAI_TransferSendLoopEDMA(DEMO_SAI1, &txHandle1, &saiTxTransfer1[0], BUFFER_NUMBER);
    SAI_TransferSendLoopEDMA(DEMO_SAI2, &txHandle2, &saiTxTransfer2[0], BUFFER_NUMBER);

    PRINTF("TDM transfers started.\r\n");
}

/*!
 * @brief Process TDM channels
 *
 * This function takes the source audio data and prepares the reversed channel order buffer.
 * The normal channel order data is kept as a pointer to the source buffer.
 * - srcBuf: Pointer to source buffer with normal channel order (1-8)
 * - destBuf: Pointer to destination buffer for reversed channel order (8-1)
 */
static void DEMO_ProcessTDMChannels(uint8_t *srcBuf, uint8_t *destBuf, uint32_t size)
{
    uint32_t i;
    uint32_t frameSize = DEMO_SAI_CHANNEL_NUM * (DEMO_SAI_BITWIDTH / 8);
    uint32_t channelSize = DEMO_SAI_BITWIDTH / 8;
    uint32_t frameCount = size / frameSize;

    /* Process each frame */
    for (i = 0; i < frameCount; i++)
    {
        uint32_t frameOffset = i * frameSize;
        uint32_t channel;

        /* Process each channel in the frame */
        for (channel = 0; channel < DEMO_SAI_CHANNEL_NUM; channel++)
        {
            uint32_t srcChannelOffset = frameOffset + (channel * channelSize);
            uint32_t destReverseOffset = frameOffset + ((DEMO_SAI_CHANNEL_NUM - 1 - channel) * channelSize);

            /* Copy channel data for reverse order (destBuf) */
            memcpy(&destBuf[destReverseOffset], &srcBuf[srcChannelOffset], channelSize);
        }
    }
}

#if defined(SAI_ErrorIRQHandler)
void SAI_ErrorIRQHandler(void)
{
    /* Clear the FIFO error flag */
    SAI_TxClearStatusFlags(DEMO_SAI1, kSAI_FIFOErrorFlag);
    SAI_TxClearStatusFlags(DEMO_SAI2, kSAI_FIFOErrorFlag);

    /* Reset FIFO */
    SAI_TxSoftwareReset(DEMO_SAI1, kSAI_ResetTypeFIFO);
    SAI_TxSoftwareReset(DEMO_SAI2, kSAI_ResetTypeFIFO);

    SDK_ISR_EXIT_BARRIER;
}
#endif
