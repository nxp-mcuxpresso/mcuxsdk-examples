/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_qsci.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ECHO_BUFFER_LENGTH 8

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* QSCI user callback */
void QSCI_UserCallback(qsci_transfer_handle_t *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/
qsci_transfer_handle_t g_qsciHandle;

uint8_t g_tipString[] =
    "QSCI interrupt example\r\nBoard receives 8 characters then sends them out\r\nDo not input more than 8 bytes of "
    "characters at one time!\r\nNow please input:\r\n";

uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] = {0};
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};
volatile bool rxBufferEmpty            = true;
volatile bool txBufferFull             = false;
volatile bool txOnGoing                = false;
volatile bool rxOnGoing                = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* QSCI user callback */
void QSCI_UserCallback(qsci_transfer_handle_t *handle)
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
    qsci_config_t config;
    qsci_transfer_t xfer;
    qsci_transfer_t sendXfer;
    qsci_transfer_t receiveXfer;

    BOARD_InitHardware();

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
    QSCI_GetDefaultConfig(&config, BOARD_DEBUG_UART_BAUDRATE, DEMO_QSCI_CLK_FREQ);
    QSCI_Init(DEMO_QSCI, &config);
    QSCI_TransferCreateHandle(DEMO_QSCI, &g_qsciHandle, QSCI_UserCallback, NULL);

    /* Send g_tipString out. */
    xfer.pu8Data     = g_tipString;
    xfer.u32DataSize = sizeof(g_tipString) - 1;
    txOnGoing        = true;
    QSCI_TransferSendNonBlocking(&g_qsciHandle, &xfer);

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
            QSCI_TransferReceiveNonBlocking(&g_qsciHandle, &receiveXfer, NULL);
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            QSCI_TransferSendNonBlocking(&g_qsciHandle, &sendXfer);
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
