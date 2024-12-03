/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "app.h"
#include "fsl_esai_edma.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "fsl_codec_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OVER_SAMPLE_RATE (256U)
#define SAMPLE_RATE      (kESAI_SampleRate48KHz)
#define BUFFER_SIZE      (1024)
#define BUFFER_NUM       (4)
#define PLAY_COUNT       (5000)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void txCallback(ESAI_Type *base, esai_edma_handle_t *handle, status_t status, void *userData);
static void rxCallback(ESAI_Type *base, esai_edma_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_QUICKACCESS_SECTION_DATA(esai_edma_handle_t txHandle);
AT_QUICKACCESS_SECTION_DATA(esai_edma_handle_t rxHandle);
AT_NONCACHEABLE_SECTION_INIT(edma_handle_t txDmaHandle)                                   = {0};
AT_NONCACHEABLE_SECTION_INIT(edma_handle_t rxDmaHandle)                                   = {0};
AT_NONCACHEABLE_SECTION_ALIGN_INIT(static uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM], 4) = {0};
volatile bool istxFinished                                                                = false;
volatile bool isrxFinished                                                                = false;
volatile uint32_t beginCount                                                              = 0;
volatile uint32_t sendCount                                                               = 0;
volatile uint32_t receiveCount                                                            = 0;
codec_handle_t codecHandle;
extern codec_config_t boardCodecConfig;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void txCallback(ESAI_Type *base, esai_edma_handle_t *handle, status_t status, void *userData)
{
    esai_transfer_t xfer = {0};

    sendCount++;

    if (sendCount == beginCount)
    {
        istxFinished = true;
        ESAI_TransferAbortSendEDMA(base, handle);
        sendCount = 0;
    }
    else
    {
        xfer.data     = audioBuff + ((sendCount - 1U) % BUFFER_NUM) * BUFFER_SIZE;
        xfer.dataSize = BUFFER_SIZE;
        ESAI_TransferSendEDMA(base, handle, &xfer);
    }
}

static void rxCallback(ESAI_Type *base, esai_edma_handle_t *handle, status_t status, void *userData)
{
    esai_transfer_t xfer = {0};

    receiveCount++;

    if (receiveCount == beginCount)
    {
        isrxFinished = true;
        ESAI_TransferAbortReceiveEDMA(base, handle);
        receiveCount = 0;
    }
    else
    {
        xfer.data     = audioBuff + ((receiveCount - 1U) % BUFFER_NUM) * BUFFER_SIZE;
        xfer.dataSize = BUFFER_SIZE;
        ESAI_TransferReceiveEDMA(base, handle, &xfer);
    }
}

int main(void)
{
    edma_config_t dmaConfig = {0};
    esai_config_t config;
    uint32_t hclkSourceClockHz = 0U;
    esai_transfer_t txfer;
    esai_transfer_t rxfer;
    esai_format_t format;

    BOARD_InitHardware();

    PRINTF("\r\nESAI EDMA example started! \r\n");

    /* Init codec */
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        PRINTF("CODEC_Init failed!\r\n");
        return -1;
    }

    /* Initialize EDMA */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);

    /* Create handle for dma channels */
    EDMA_CreateHandle(&txDmaHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
    EDMA_CreateHandle(&rxDmaHandle, EXAMPLE_DMA, EXAMPLE_RX_CHANNEL);

    ESAI_GetDefaultConfig(&config);
#if defined(CODEC_CS42888)
    config.syncMode = kESAI_ModeAsync;
#endif
    ESAI_Init(EXAMPLE_ESAI, &config);

    /* Configure the audio format */
    format.slotType      = kESAI_SlotLen32WordLen16;
    format.sampleRate_Hz = SAMPLE_RATE;
    format.sectionMap    = 0x1;

    ESAI_TransferTxCreateHandleEDMA(EXAMPLE_ESAI, &txHandle, txCallback, NULL, &txDmaHandle);
    ESAI_TransferRxCreateHandleEDMA(EXAMPLE_ESAI, &rxHandle, rxCallback, NULL, &rxDmaHandle);

    hclkSourceClockHz = ESAI_SOURCE_CLOCK_FREQ;
#if defined ESAI_TX_CHANNEL
    format.sectionMap = (1U << ESAI_TX_CHANNEL);
#endif
    ESAI_TransferTxSetFormatEDMA(EXAMPLE_ESAI, &txHandle, &format, format.sampleRate_Hz * 256U, hclkSourceClockHz);
#if defined ESAI_RX_CHANNEL
    format.sectionMap = (1U << ESAI_RX_CHANNEL);
#else
    format.sectionMap = 1U;
#endif
    ESAI_TransferRxSetFormatEDMA(EXAMPLE_ESAI, &rxHandle, &format, format.sampleRate_Hz * 256U, hclkSourceClockHz);

    /*  xfer structure */
    rxfer.data     = audioBuff;
    rxfer.dataSize = BUFFER_SIZE;
    txfer.data     = audioBuff;
    txfer.dataSize = BUFFER_SIZE;

    /* Set the time to record and playback */
    beginCount = PLAY_COUNT;

    ESAI_TransferSendEDMA(EXAMPLE_ESAI, &txHandle, &txfer);
    ESAI_TransferReceiveEDMA(EXAMPLE_ESAI, &rxHandle, &rxfer);

    /* Waiting for transfer finished */
    while ((isrxFinished == false) || (istxFinished == false))
    {
    }

    ESAI_TransferAbortReceiveEDMA(EXAMPLE_ESAI, &rxHandle);
    ESAI_TransferAbortSendEDMA(EXAMPLE_ESAI, &txHandle);

    PRINTF("\r\nESAI EDMA example succeed! \r\n");

    while (1)
    {
    }
}
