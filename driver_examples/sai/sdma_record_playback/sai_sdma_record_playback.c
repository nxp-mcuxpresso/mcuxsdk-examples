/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_sai_sdma.h"
#include "fsl_codec_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_SIZE      (1024U)
#define BUFFER_NUMBER    (4U)
#define OVER_SAMPLE_RATE (384U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void txCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(sai_sdma_handle_t txHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sai_sdma_handle_t rxHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t txDmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t rxDmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t txContext, 4);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t rxContext, 4);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t Buffer[BUFFER_NUMBER * BUFFER_SIZE], 4);
extern codec_config_t boardCodecConfig;
codec_handle_t codecHandle;
volatile uint32_t emptyBlock = BUFFER_NUMBER;
static uint32_t tx_index = 0U, rx_index = 0U;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void txCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxIdle == status)
    {
        emptyBlock++;
    }
}

static void rxCallback(I2S_Type *base, sai_sdma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_RxIdle == status)
    {
        emptyBlock--;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    sai_transfer_t xfer;
    sdma_config_t dmaConfig = {0};
    sai_transceiver_t saiConfig;

    BOARD_InitHardware();

    PRINTF("SAI SDMA record playback example started!\n\r");
    /* Create SDMA handle */
    SDMA_GetDefaultConfig(&dmaConfig);
#ifdef SDMA_FREQ_EQUALS_ARM
    dmaConfig.ratio = kSDMA_ARMClockFreq;
#endif

    SDMA_Init(DEMO_DMA, &dmaConfig);
    SDMA_CreateHandle(&txDmaHandle, DEMO_DMA, DEMO_TX_SDMA_CHANNEL, &txContext);
    SDMA_SetChannelPriority(DEMO_DMA, DEMO_TX_SDMA_CHANNEL, 3);
    SDMA_CreateHandle(&rxDmaHandle, DEMO_DMA, DEMO_RX_SDMA_CHANNEL, &rxContext);
    SDMA_SetChannelPriority(DEMO_DMA, DEMO_RX_SDMA_CHANNEL, 2);
    /* SAI init */
    SAI_Init(DEMO_SAI);
    SAI_TransferTxCreateHandleSDMA(DEMO_SAI, &txHandle, txCallback, NULL, &txDmaHandle, DEMO_SAI_SDMA_TX_SOURCE);
    SAI_TransferRxCreateHandleSDMA(DEMO_SAI, &rxHandle, rxCallback, NULL, &rxDmaHandle, DEMO_SAI_SDMA_RX_SOURCE);

    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&saiConfig, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    saiConfig.syncMode            = DEMO_SAI_TX_SYNC_MODE;
    saiConfig.bitClock.bclkSource = (sai_bclk_source_t)DEMO_SAI_CLOCK_SOURCE;
    SAI_TransferTxSetConfigSDMA(DEMO_SAI, &txHandle, &saiConfig);
    saiConfig.syncMode = DEMO_SAI_RX_SYNC_MODE;
    SAI_TransferRxSetConfigSDMA(DEMO_SAI, &rxHandle, &saiConfig);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
    SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

    /* Use default setting to init codec */
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }

    while (1)
    {
        if (emptyBlock > 0)
        {
            xfer.data     = Buffer + rx_index * BUFFER_SIZE;
            xfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferReceiveSDMA(DEMO_SAI, &rxHandle, &xfer))
            {
                rx_index++;
            }
            if (rx_index == BUFFER_NUMBER)
            {
                rx_index = 0U;
            }
        }
        if (emptyBlock < BUFFER_NUMBER)
        {
            xfer.data     = Buffer + tx_index * BUFFER_SIZE;
            xfer.dataSize = BUFFER_SIZE;
            if (kStatus_Success == SAI_TransferSendSDMA(DEMO_SAI, &txHandle, &xfer))
            {
                tx_index++;
            }
            if (tx_index == BUFFER_NUMBER)
            {
                tx_index = 0U;
            }
        }
    }
}

void SAI_UserIRQHandler(void)
{
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);
    SDK_ISR_EXIT_BARRIER;
}
