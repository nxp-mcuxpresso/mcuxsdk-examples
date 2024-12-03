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

#define RX_RING_BUFFER_SIZE 20U
#define ECHO_BUFFER_SIZE    8U
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
    "QSCI RX ring buffer example\r\nSend back received data\r\nEcho every 8 bytes\r\nDo not input more than 20 bytes "
    "of characters at one time!\r\n";
uint8_t g_rxRingBuffer[RX_RING_BUFFER_SIZE] = {0}; /* RX ring buffer. */

uint8_t g_rxBuffer[ECHO_BUFFER_SIZE] = {0}; /* Buffer for receive data to echo. */
uint8_t g_txBuffer[ECHO_BUFFER_SIZE] = {0}; /* Buffer for send data to echo. */
volatile bool rxBufferEmpty          = true;
volatile bool txBufferFull           = false;
volatile bool txOnGoing              = false;
volatile bool rxOnGoing              = false;

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
    uint32_t receivedBytes = 0U;
    uint32_t i;

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
    QSCI_TransferStartRingBuffer(&g_qsciHandle, g_rxRingBuffer, RX_RING_BUFFER_SIZE);

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
    sendXfer.u32DataSize    = ECHO_BUFFER_SIZE;
    receiveXfer.pu8Data     = g_rxBuffer;
    receiveXfer.u32DataSize = ECHO_BUFFER_SIZE;

    while (1)
    {
        /* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
        if ((!rxBufferEmpty) && (!txBufferFull))
        {
            memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_SIZE);
            rxBufferEmpty = true;
            txBufferFull  = true;
        }

        /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            QSCI_TransferReceiveNonBlocking(&g_qsciHandle, &receiveXfer, &receivedBytes);
            if (ECHO_BUFFER_SIZE == receivedBytes)
            {
                rxBufferEmpty = false;
                rxOnGoing     = false;
            }
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            QSCI_TransferSendNonBlocking(&g_qsciHandle, &sendXfer);
        }

        /* Delay some time, simulate the app is processing other things, input data save to ring buffer. */
        i = 0x10U;
        while (i--)
        {
            __NOP();
        }
    }
}
