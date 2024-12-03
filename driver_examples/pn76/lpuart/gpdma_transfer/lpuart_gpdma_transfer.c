/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_lpuart_gpdma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ECHO_BUFFER_LENGTH 8

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* LPUART user callback */
void LPUART_UserCallback(LPUART_Type *base, lpuart_gpdma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

lpuart_gpdma_handle_t g_lpuartDmaHandle;
gpdma_handle_t g_lpuartTxDmaHandle;
gpdma_handle_t g_lpuartRxDmaHandle;
uint8_t g_tipString[] = "LPUART GPDMA example\r\nSend back received data\r\nEcho every 8 characters\r\n";
uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] = {0};
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};
volatile bool rxBufferEmpty            = true;
volatile bool txBufferFull             = false;
volatile bool txOnGoing                = false;
volatile bool rxOnGoing                = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* LPUART user callback */
void LPUART_UserCallback(LPUART_Type *base, lpuart_gpdma_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_LPUART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing    = false;
    }

    if (kStatus_LPUART_RxIdle == status)
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
    lpuart_config_t config;
    lpuart_data_t xfer;
    lpuart_data_t sendXfer;
    lpuart_data_t receiveXfer;

    BOARD_InitHardware();

    config.dW = 0x000F0000UL;
    config.b.BaudRate = kLPUART_BaudRate115200;

    LPUART_Init(DEMO_LPUART, &config, DEMO_LPUART_CLOCK_FREQ);

    /* Init the GPDMA module */
    GPDMA_Init(DEMO_DMA);
    GPDMA_CreateHandle(&g_lpuartTxDmaHandle, DEMO_DMA, DEMO_DMA_CHANNEL1);
    GPDMA_CreateHandle(&g_lpuartRxDmaHandle, DEMO_DMA, DEMO_DMA_CHANNEL0);

    /* Create LPUART GPDMA handle. */
    LPUART_TransferCreateHandleGPDMA(DEMO_LPUART, &g_lpuartDmaHandle, LPUART_UserCallback, NULL, &g_lpuartTxDmaHandle,
                                     &g_lpuartRxDmaHandle);

    /* Send g_tipString out. */
    xfer.data       = g_tipString;
    xfer.bufferSize = sizeof(g_tipString) - 1;
    txOnGoing       = true;

    GPDMA_SetChannelDestinationPeripheral(g_lpuartDmaHandle.txDmaHandle->base, g_lpuartDmaHandle.txDmaHandle->channel,
                                          kGpdmaRequestMuxLpuartTx);
    GPDMA_SetChannelSourcePeripheral(g_lpuartDmaHandle.rxDmaHandle->base, g_lpuartDmaHandle.rxDmaHandle->channel,
                                     kGpdmaRequestMuxLpuartRx);
    LPUART_TransferSendGPDMA(DEMO_LPUART, &g_lpuartDmaHandle, &xfer);

    /* Wait send finished */
    while (txOnGoing)
    {
    }

    /* Start to echo. */
    sendXfer.data          = g_txBuffer;
    sendXfer.bufferSize    = ECHO_BUFFER_LENGTH;
    receiveXfer.data       = g_rxBuffer;
    receiveXfer.bufferSize = ECHO_BUFFER_LENGTH;

    while (1)
    {
        /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            LPUART_TransferReceiveGPDMA(DEMO_LPUART, &g_lpuartDmaHandle, &receiveXfer);
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            LPUART_TransferSendGPDMA(DEMO_LPUART, &g_lpuartDmaHandle, &sendXfer);
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
