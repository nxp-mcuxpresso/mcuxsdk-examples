/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_qsci_dma.h"
#include "fsl_dma.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ECHO_BUFFER_LENGTH 8

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* QSCI user callback */
void QSCI_UserCallback(qsci_dma_transfer_handle_t *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/

AT_NONCACHEABLE_SECTION_INIT(uint8_t g_tipString[]) =
    "QSCI DMA example\r\nSend back received data\r\nEcho every 8 characters\r\nDo not input more than 8 bytes of "
    "characters at one time!\r\n";
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_txBuffer[ECHO_BUFFER_LENGTH]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH]) = {0};
qsci_dma_transfer_handle_t g_qsciDmaHandle;
volatile bool rxBufferEmpty = true;
volatile bool txBufferFull  = false;
volatile bool txOnGoing     = false;
volatile bool rxOnGoing     = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* QSCI user callback */
void QSCI_UserCallback(qsci_dma_transfer_handle_t *handle)
{
    status_t busStatus = QSCI_GET_BUS_STATUS(handle);

    if (kStatus_QSCI_TxIdle == busStatus)
    {
        txBufferFull = false;
        txOnGoing    = false;
    }

    if (kStatus_QSCI_RxIdle == busStatus)
    {
        rxBufferEmpty = false;
        rxOnGoing     = false;
    }
}
/*!
 * @brief Main function
 */
int main(void)
{
    qsci_config_t qsci_config;
    dma_config_t dma_config;
    qsci_transfer_t xfer;
    qsci_transfer_t sendXfer;
    qsci_transfer_t receiveXfer;

    BOARD_InitHardware();

    /* Initialize the QSCI. */
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
    QSCI_GetDefaultConfig(&qsci_config, BOARD_DEBUG_UART_BAUDRATE, EXAMPLE_QSCI_CLK_FREQ);
    QSCI_Init(EXAMPLE_QSCI, &qsci_config);

    /* Init the DMA module */
    memset(&dma_config, 0U, sizeof(dma_config));

    /* Set request source. */
    DMA_SetChannelPeripheralRequest(EXAMPLE_QSCI_DMA_BASEADDR, QSCI_TX_DMA_CHANNEL, QSCI_TX_DMA_REQUEST);
    DMA_SetChannelPeripheralRequest(EXAMPLE_QSCI_DMA_BASEADDR, QSCI_RX_DMA_CHANNEL, QSCI_RX_DMA_REQUEST);

    DMA_Init(EXAMPLE_QSCI_DMA_BASEADDR, &dma_config);

    /* Create QSCI DMA handle. */
    QSCI_TransferCreateHandleDMA(EXAMPLE_QSCI, &g_qsciDmaHandle, QSCI_UserCallback, NULL, EXAMPLE_QSCI_DMA_BASEADDR,
                                 QSCI_TX_DMA_CHANNEL, QSCI_RX_DMA_CHANNEL);

    /* Send g_tipString out. */
    xfer.pu8Data     = g_tipString;
    xfer.u32DataSize = sizeof(g_tipString) - 1;
    txOnGoing        = true;
    QSCI_TransferSendDMA(&g_qsciDmaHandle, &xfer);

    /* Wait send finished */
    while (txOnGoing)
    {
    }

    /* Start to echo. */
    sendXfer.pu8Data        = g_txBuffer;
    sendXfer.u32DataSize    = ECHO_BUFFER_LENGTH;
    receiveXfer.pu8Data     = g_rxBuffer;
    receiveXfer.u32DataSize = ECHO_BUFFER_LENGTH;

    while (1)
    {
        /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            QSCI_TransferReceiveDMA(&g_qsciDmaHandle, &receiveXfer);
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            QSCI_TransferSendDMA(&g_qsciDmaHandle, &sendXfer);
        }

        /* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
        if ((!rxBufferEmpty) && (!txBufferFull))
        {
            memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_LENGTH);
            rxBufferEmpty = true;
            txBufferFull  = true;
        }
    }
}
