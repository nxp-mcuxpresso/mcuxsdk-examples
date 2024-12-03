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

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 16

/*! @brief Ring buffer to save received data. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_tipString[] =
    "Usart functional API interrupt example\r\nBoard receives characters then sends them out\r\nNow please input:\r\n";

/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == txIndex)
  Ring buffer empty: (rxIndex == txIndex)
*/
uint8_t demoRingBuffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */

/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_USART_IRQHandler(void)
{
    uint8_t data;

    if ((USART_IsRxFifoEnable(DEMO_USART)) ?
            (kUSART_RxFifoThresholdFlag & USART_GetFifoStatusFlags(DEMO_USART)) :
            ((kUSART_RxReady | kUSART_HardwareOverrunFlag) & USART_GetStatusFlags(DEMO_USART)))
    {
        data = USART_ReadByte(DEMO_USART);
        /* If ring buffer is not full, add data to ring buffer. */
        if (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) != txIndex)
        {
            demoRingBuffer[rxIndex] = data;
            rxIndex++;
            rxIndex %= DEMO_RING_BUFFER_SIZE;
        }
    }
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    usart_config_t config;

    BOARD_InitHardware();

    /* Default config:
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUSART_ParityDisabled;
     * config.stopBitCount = kUSART_OneStopBit;
     * config.loopback = false;
     * config.fifoConfig.txFifoThreshold = 0;
     * config.fifoConfig.rxFifoThreshold = 0;
     * config.fifoConfig.TxFifoSize = 16U;
     * config.fifoConfig.RxFifoSize = 16U;
     * Note: With the system FIFO used, an interrupt will be triggered if the data
     * in the receive fifo is larger then the receive threshold value.
     */
    USART_GetDefaultConfig(&config);
    config.enableRx                = true;
    config.enableTx                = true;
    config.fifoConfig.enableRxFifo = true;
    config.fifoConfig.enableTxFifo = false;
    config.baudRate_Bps            = BOARD_DEBUG_UART_BAUDRATE;

    USART_Init(DEMO_USART, &config, DEMO_USART_CLK_FREQ);

    /* Send g_tipString out. */
    USART_WriteBlocking(DEMO_USART, g_tipString, (sizeof(g_tipString) / sizeof(g_tipString[0])) - 1);

    /* Enable RX interrupt. */
    if (USART_IsRxFifoEnable(DEMO_USART))
    {
        USART_EnableFifoInterrupts(DEMO_USART, kUSART_RxFifoThresholdInterruptEnable);
    }
    else
    {
        USART_EnableInterrupts(DEMO_USART, kUSART_RxReadyInterruptEnable | kUSART_HardwareOverRunInterruptEnable);
    }
    EnableIRQ(DEMO_USART_IRQn);

    while (1)
    {
        /* Send data only when USART TX register is empty and ring buffer has data to send out. */
        while ((kUSART_TxReady & USART_GetStatusFlags(DEMO_USART)) && (rxIndex != txIndex))
        {
            USART_WriteByte(DEMO_USART, demoRingBuffer[txIndex]);
            txIndex++;
            txIndex %= DEMO_RING_BUFFER_SIZE;
        }
    }
}
