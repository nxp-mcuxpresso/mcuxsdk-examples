/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_qsci_dma.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Ring buffer size definition, please make sure to set this value large enough.
 * Otherwise, once overflow occurred, data in ring buffer will be overwritten.
 */
#define EXAMPLE_RING_BUFFER_SIZE (32U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Initialzie the USART module. */
static void EXAMPLE_InitDMA(void);

/* Initalize the DMA configuration for USART  TX and RX used.  */
static void EXAMPLE_InitQSCI(void);

/* Start ring buffer. */
static void EXAMPLE_StartRingBufferDMA(void);

/* Reading data from the ring buffer. */
static void EXAMPLE_ReadRingBuffer(uint8_t *ringBuffer, uint8_t *receiveBuffer, uint32_t length);

/* QSCI RX EDMA call back. */
void EXAMPLE_DmaRxCallback(dma_handle_t *handle, void *param);

/* QSCI user callback */
void EXAMPLE_QsciDmaCallback(qsci_dma_transfer_handle_t *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/
qsci_dma_transfer_handle_t g_qsciDmaHandle;
dma_handle_t g_qsciRxDmaHandle;
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_tipString[]) =
    "QSCI DMA ring buffer example.\r\nBoard will send back received characters.\r\nDo not input more than 32 bytes of "
    "characters at one time!\r\n";
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_rxBuffer[EXAMPLE_RING_BUFFER_SIZE]) = {0};
volatile bool txOnGoing                                                    = false;
volatile bool isIdleLineDetected                                           = false;
volatile uint32_t ringBufferFlag                                           = 0U;
volatile uint32_t receivedBytes                                            = 0U;
volatile uint32_t ringBufferIndex                                          = 0U;

/* allocate ring buffer section. */
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_ringBuffer[EXAMPLE_RING_BUFFER_SIZE]) = {0};
/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialzie the USART module. */
static void EXAMPLE_InitQSCI(void)
{
    qsci_config_t config;

    /*
      config.eTransferMode     = kQSCI_Normal;
      config.bStopInWaitEnable = false;
      config.eDataBitMode      = kQSCI_Data8Bit;
      config.eWakeupMode       = kQSCI_WakeupOnIdleLine;
      config.ePolarityMode     = kQSCI_PolarityNormal;
      config.eParityMode       = kQSCI_ParityDisabled;
      config.bEnableStopHold   = false;
      config.bEnableTx         = false;
      config.bEnableRx         = false;
      config.bEnableFifo  = true;
      config.bEnableTxDMA = false;
      config.bEnableRxDMA = false;
      config.eTxFifoWatermark = kQSCI_TxWater3Word;
      config.eRxFifoWatermark = kQSCI_RxWater1Word;
      config.u32BaudRateBps  = 115200;
      config.u32SrcClockHz   = u32SrcClockHz;
      config.u8Interrupts = 0U;
     */
    QSCI_GetDefaultConfig(&config, BOARD_DEBUG_UART_BAUDRATE, EXAMPLE_QSCI_CLK_FREQ);
    QSCI_Init(EXAMPLE_QSCI, &config);
}

/* Initalize the DMA configuration for USART  TX and RX used. */
static void EXAMPLE_InitDMA(void)
{
    dma_config_t dma_config;
    memset(&dma_config, 0U, sizeof(dma_config));

    /* Set request source. */
    DMA_SetChannelPeripheralRequest(EXAMPLE_QSCI_DMA_BASEADDR, QSCI_TX_DMA_CHANNEL, QSCI_TX_DMA_REQUEST);
    DMA_SetChannelPeripheralRequest(EXAMPLE_QSCI_DMA_BASEADDR, QSCI_RX_DMA_CHANNEL, QSCI_RX_DMA_REQUEST);

    /* Init the DMA module */
    DMA_Init(EXAMPLE_QSCI_DMA_BASEADDR, &dma_config);

    /* Create QSCI DMA handle for sending data. */
    QSCI_TransferCreateHandleDMA(EXAMPLE_QSCI, &g_qsciDmaHandle, EXAMPLE_QsciDmaCallback, NULL,
                                 EXAMPLE_QSCI_DMA_BASEADDR, QSCI_TX_DMA_CHANNEL, QSCI_RX_DMA_CHANNEL);
}

/* Start ring buffer. */
static void EXAMPLE_StartRingBufferDMA(void)
{
    dma_channel_transfer_config_t xferConfig;

    uint32_t u32SrcByteAddr = QSCI_GetDataRegisterAddress(EXAMPLE_QSCI);
    DMA_GetChannelDefaultTransferConfig(&xferConfig, u32SrcByteAddr, (uint32_t)(uint8_t *)g_ringBuffer,
                                        EXAMPLE_RING_BUFFER_SIZE, kDMA_ChannelTransferWidth8Bits,
                                        kDMA_ChannelTransferPeripheralToMemory);
    /* Config  transfer. */
    xferConfig.bEnablAutoStopPeripheralRequest = true;
    xferConfig.bEnableCycleSteal               = true;
    DMA_TransferChannelSubmit(&g_qsciRxDmaHandle, &xferConfig);
    DMA_TransferChannelStart(&g_qsciRxDmaHandle, true);
    /* Enable QSCI RX EDMA. */
    QSCI_EnableRx(EXAMPLE_QSCI, true);
    QSCI_EnableRxDMA(EXAMPLE_QSCI, true);
}

/* Reading out the data from ring buffer. */
static void EXAMPLE_ReadRingBuffer(uint8_t *ringBuffer, uint8_t *receiveBuffer, uint32_t length)
{
    assert(ringBuffer != NULL);
    assert(receiveBuffer != NULL);
    assert(length != 0U);

    uint32_t index = length;

    /* If length if larger than ring buffer size, it means overflow occurred, need to reset the ringBufferIndex. */
    if (length > EXAMPLE_RING_BUFFER_SIZE)
    {
        ringBufferIndex = ((ringBufferIndex + length) % EXAMPLE_RING_BUFFER_SIZE);
        index           = EXAMPLE_RING_BUFFER_SIZE;
    }

    while (index)
    {
        *(receiveBuffer++) = ringBuffer[ringBufferIndex++];
        if (ringBufferIndex == EXAMPLE_RING_BUFFER_SIZE)
        {
            ringBufferIndex = 0U;
            ringBufferFlag--;
        }
        index--;
    }
}

/* QSCI RX EDMA call back. */
void EXAMPLE_DmaRxCallback(dma_handle_t *handle, void *param)
{
    ringBufferFlag++;
    EXAMPLE_StartRingBufferDMA();
}

/* QSCI EDMA TX user callback */
void EXAMPLE_QsciDmaCallback(qsci_dma_transfer_handle_t *handle)
{
    status_t busStatus = QSCI_GET_BUS_STATUS(handle);

    if (kStatus_QSCI_TxIdle == busStatus)
    {
        txOnGoing = false;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    qsci_transfer_t sendXfer;

    BOARD_InitHardware();

    /* Initialize the QSCI module. */
    EXAMPLE_InitQSCI();

    /* Intialzie the DMA configuration. */
    EXAMPLE_InitDMA();

    /* Send g_tipString out. */
    sendXfer.pu8Data     = g_tipString;
    sendXfer.u32DataSize = sizeof(g_tipString) - 1;
    txOnGoing            = true;
    QSCI_TransferSendDMA(&g_qsciDmaHandle, &sendXfer);

    /* Wait send finished */
    while (txOnGoing)
    {
    }

    /* Configure RX. */
    DMA_TransferChannelCreateHandle(EXAMPLE_QSCI_DMA_BASEADDR, &g_qsciRxDmaHandle, QSCI_RX_DMA_CHANNEL,
                                    EXAMPLE_DmaRxCallback, NULL);

    /* Start ring buffer with DMA used. */
    EXAMPLE_StartRingBufferDMA();

    while (1)
    {
        /* If new data arrived. */
        while (0U == ((uint16_t)kQSCI_RxIdleLineFlag & QSCI_GetStatusFlags(EXAMPLE_QSCI)))
        {
        }
        (void)QSCI_ClearStatusFlags(EXAMPLE_QSCI, kQSCI_RxIdleLineFlag);
        receivedBytes =
            EXAMPLE_RING_BUFFER_SIZE - DMA_GetChannelRemainingBytes(EXAMPLE_QSCI_DMA_BASEADDR, QSCI_RX_DMA_CHANNEL);
        receivedBytes += (EXAMPLE_RING_BUFFER_SIZE * ringBufferFlag) - ringBufferIndex;

        EXAMPLE_ReadRingBuffer(g_ringBuffer, g_rxBuffer, receivedBytes);
        /* Start to echo. */
        sendXfer.pu8Data     = g_rxBuffer;
        sendXfer.u32DataSize = receivedBytes;

        QSCI_TransferSendDMA(&g_qsciDmaHandle, &sendXfer);
    }
}
