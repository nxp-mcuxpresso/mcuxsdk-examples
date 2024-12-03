/*
 * Copyright  2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_usart.h"
#include "fsl_usart_dma.h"
#include "fsl_dma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ECHO_BUFFER_LENGTH 8

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* UART user callback */
void UART_UserCallback(USART_Type *base, usart_dma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

usart_dma_handle_t g_uartDmaHandle;
dma_handle_t g_uartTxDmaHandle;
dma_handle_t g_uartRxDmaHandle;
uint8_t g_tipString[]                  = "USART DMA example\r\nSend back received data\r\nEcho every 8 characters\r\n";
uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] = {0};
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};
volatile bool rxBufferEmpty            = true;
volatile bool txBufferFull             = false;
volatile bool txOnGoing                = false;
volatile bool rxOnGoing                = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* UART user callback */
void USART_UserCallback(USART_Type *base, usart_dma_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_USART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing    = false;
    }

    if (kStatus_USART_RxIdle == status)
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
    usart_config_t config;
    usart_transfer_t xfer;
    usart_transfer_t sendXfer;
    usart_transfer_t receiveXfer;

    BOARD_InitHardware();

    /* Default Configuration:
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.fifoConfig.txFifoThreshold = 0;
     * config.fifoConfig.rxFifoThreshold = 0;
     * config.fifoConfig.TxFifoSize = 16U;
     * config.fifoConfig.RxFifoSize = 16U;
     * Note: If you have decided to enable the system FIFO buffer for DMA transfer, please
     * do not set the FIFO buffer size to zero, this will cause logic error.
     */
    USART_GetDefaultConfig(&config);
    config.baudRate_Bps            = BOARD_DEBUG_UART_BAUDRATE;
    config.enableRx                = true;
    config.enableTx                = true;
    config.fifoConfig.enableRxFifo = true;
    config.fifoConfig.enableTxFifo = false;
    USART_Init(DEMO_USART, &config, DEMO_USART_CLK_FREQ);

    /* Configure DMA. */
    DMA_Init(DMA0);
    DMA_EnableChannel(DMA0, USART_TX_DMA_CHANNEL);
    DMA_EnableChannel(DMA0, USART_RX_DMA_CHANNEL);

    DMA_CreateHandle(&g_uartTxDmaHandle, DMA0, USART_TX_DMA_CHANNEL);
    DMA_CreateHandle(&g_uartRxDmaHandle, DMA0, USART_RX_DMA_CHANNEL);

    /* Create UART DMA handle. */
    USART_TransferCreateHandleDMA(DEMO_USART, &g_uartDmaHandle, USART_UserCallback, NULL, &g_uartTxDmaHandle,
                                  &g_uartRxDmaHandle);

    /* Send g_tipString out. */
    xfer.data     = g_tipString;
    xfer.dataSize = sizeof(g_tipString) - 1;
    txOnGoing     = true;
    USART_TransferSendDMA(DEMO_USART, &g_uartDmaHandle, &xfer);

    /* Wait send finished */
    while (txOnGoing)
    {
    }

    /* Start to echo. */
    sendXfer.data        = g_txBuffer;
    sendXfer.dataSize    = sizeof(g_txBuffer);
    receiveXfer.data     = g_rxBuffer;
    receiveXfer.dataSize = sizeof(g_rxBuffer);

    while (1)
    {
        /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            USART_TransferReceiveDMA(DEMO_USART, &g_uartDmaHandle, &receiveXfer);
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            USART_TransferSendDMA(DEMO_USART, &g_uartDmaHandle, &sendXfer);
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
