/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_pdm.h"
#include "fsl_pdm_sdma.h"
#include "fsl_sdma.h"
#include "fsl_sai.h"
#include "fsl_sai_sdma.h"
#include "fsl_debug_console.h"
#include "fsl_sdma_script.h"
#include "fsl_codec_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_SIZE   (1024)
#define BUFFER_NUMBER (4)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void pdmSdmallback(PDM_Type *base, pdm_sdma_handle_t *handle, status_t status, void *userData);
static void saiCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(pdm_sdma_handle_t s_pdmRxHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t s_pdmDmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t s_saiDmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sai_sdma_handle_t s_saiTxHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t s_pdmSdmaContext, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t s_saiSdmaContext, 4);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_buffer[BUFFER_SIZE * BUFFER_NUMBER], 4);
static volatile uint32_t s_bufferValidBlock = BUFFER_NUMBER;
static volatile uint32_t s_readIndex        = 0U;
static volatile uint32_t s_writeIndex       = 0U;
static const pdm_config_t pdmConfig         = {
#if defined(FSL_FEATURE_PDM_HAS_DECIMATION_FILTER_BYPASS) && FSL_FEATURE_PDM_HAS_DECIMATION_FILTER_BYPASS
    .enableFilterBypass = false,
#endif
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
    .gain = kPDM_DfOutputGain4,
};

codec_handle_t codecHandle;

extern codec_config_t boardCodecConfig;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void pdmSdmallback(PDM_Type *base, pdm_sdma_handle_t *handle, status_t status, void *userData)
{
    s_bufferValidBlock--;
}

static void saiCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxError == status)
    {
        /* Handle the error. */
    }
    else
    {
        s_bufferValidBlock++;
    }
}

void PDM_ERROR_IRQHandler(void)
{
    uint32_t fifoStatus = 0U;
#if (defined(FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ) && (FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ == 1U))
    if (PDM_GetStatus(DEMO_PDM) & PDM_STAT_LOWFREQF_MASK)
    {
        PDM_ClearStatus(DEMO_PDM, PDM_STAT_LOWFREQF_MASK);
    }
#endif
    fifoStatus = PDM_GetFifoStatus(DEMO_PDM);
    if (fifoStatus)
    {
        PDM_ClearFIFOStatus(DEMO_PDM, fifoStatus);
    }
    SDK_ISR_EXIT_BARRIER;
}

void SAI_UserIRQHandler(void)
{
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    pdm_transfer_t pdmXfer;
    sdma_config_t dmaConfig = {0};
    sai_transfer_t saiXfer;
    sai_transceiver_t config;

    BOARD_InitHardware();

    PRINTF("PDM SAI sdma example started!\n\r");

    /* Create SDMA handle */
    SDMA_GetDefaultConfig(&dmaConfig);
    dmaConfig.ratio = kSDMA_ARMClockFreq;

    SDMA_Init(DEMO_PDM_DMA, &dmaConfig);
    SDMA_Init(DEMO_SAI_DMA, &dmaConfig);
    SDMA_CreateHandle(&s_pdmDmaHandle, DEMO_PDM_DMA, DEMO_PDM_DMA_CHANNEL, &s_pdmSdmaContext);
    SDMA_SetChannelPriority(DEMO_PDM_DMA, DEMO_PDM_DMA_CHANNEL, DEMO_PDM_DMA_CHANNEL_PRIORITY);

    SDMA_CreateHandle(&s_saiDmaHandle, DEMO_SAI_DMA, DEMO_SAI_DMA_CHANNEL, &s_saiSdmaContext);
    SDMA_SetChannelPriority(DEMO_SAI_DMA, DEMO_SAI_DMA_CHANNEL, DEMO_SAI_DMA_CHANNEL_PRIORITY);

    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleSDMA(DEMO_SAI, &s_saiTxHandle, saiCallback, NULL, &s_saiDmaHandle,
                                   DEMO_SAI_TX_DMA_REQUEST_SOURCE);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);

    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }

    config.bitClock.bclkSource = DEMO_SAI_CLOCK_SOURCE;
    SAI_TransferTxSetConfigSDMA(DEMO_SAI, &s_saiTxHandle, &config);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MasterClockConfig();

    /* Setup pdm */
    PDM_Init(DEMO_PDM, &pdmConfig);
    PDM_TransferCreateHandleSDMA(DEMO_PDM, &s_pdmRxHandle, pdmSdmallback, NULL, &s_pdmDmaHandle,
                                 DEMO_PDM_DMA_REQUEST_SOURCE);
    PDM_SetChannelConfigSDMA(DEMO_PDM, &s_pdmRxHandle, DEMO_PDM_ENABLE_CHANNEL_LEFT, &channelConfig);
    PDM_SetChannelConfigSDMA(DEMO_PDM, &s_pdmRxHandle, DEMO_PDM_ENABLE_CHANNEL_RIGHT, &channelConfig);
    PDM_SetSampleRate(DEMO_PDM, (1U << DEMO_PDM_ENABLE_CHANNEL_LEFT) | (1U << DEMO_PDM_ENABLE_CHANNEL_RIGHT),
                      pdmConfig.qualityMode, pdmConfig.cicOverSampleRate,
                      DEMO_PDM_CLK_FREQ / DEMO_PDM_SAMPLE_CLOCK_RATE);
    PDM_Reset(DEMO_PDM);

    while (1)
    {
        /* wait one buffer idle to recieve data */
        if (s_bufferValidBlock > 0)
        {
            pdmXfer.data     = (uint8_t *)((uint32_t)s_buffer + s_readIndex * BUFFER_SIZE);
            pdmXfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == PDM_TransferReceiveSDMA(DEMO_PDM, &s_pdmRxHandle, &pdmXfer))
            {
                s_readIndex++;
            }
            if (s_readIndex == BUFFER_NUMBER)
            {
                s_readIndex = 0U;
            }
        }
        /* wait one buffer busy to send data */
        if (s_bufferValidBlock < BUFFER_NUMBER)
        {
            saiXfer.data     = (uint8_t *)((uint32_t)s_buffer + s_writeIndex * BUFFER_SIZE);
            saiXfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferSendSDMA(DEMO_SAI, &s_saiTxHandle, &saiXfer))
            {
                s_writeIndex++;
            }
            if (s_writeIndex == BUFFER_NUMBER)
            {
                s_writeIndex = 0U;
            }
        }
    }
}
