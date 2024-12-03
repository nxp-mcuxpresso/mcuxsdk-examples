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

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 16

/*! @brief Ring buffer to save received data. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DEMO_QSCI_IRQHandler(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_tipString[] =
    "QSCI functional API interrupt example\r\nBoard receives characters then sends them out\r\nDo not input more than "
    "16 bytes of characters at one time!\r\nNow please input:\r\n";

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

#pragma interrupt alignsp saveall
void DEMO_QSCI_IRQHandler(void)
{
    uint8_t data;
    uint16_t tmprxIndex = rxIndex;
    uint16_t tmptxIndex = txIndex;

    /* If new data arrived. */
    data = QSCI_ReadByte(DEMO_QSCI);

    /* If ring buffer is not full, add data to ring buffer. */
    if (((tmprxIndex + 1) % DEMO_RING_BUFFER_SIZE) != tmptxIndex)
    {
        demoRingBuffer[rxIndex] = data;
        rxIndex++;
        rxIndex %= DEMO_RING_BUFFER_SIZE;
    }
}
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    qsci_config_t config;
    uint16_t tmprxIndex = rxIndex;
    uint16_t tmptxIndex = txIndex;

    BOARD_InitHardware();

    /*
      config.eTransferMode     = kQSCI_Normal;
      config.bStopInWaitEnable = false;
      config.eDataBitMode      = kQSCI_Data8Bit;
      config.eWakeupMode       = kQSCI_WakeupOnIdleLine;
      config.ePolarityMode     = kQSCI_PolarityNormal;
      config.eParityMode       = kQSCI_ParityDisabled;
      config.bEnableStopHold   = false;
      config.bEnableTx         = true;
      config.bEnableRx         = true;
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
    config.bEnableTx = true;
    config.bEnableRx = true;
    QSCI_Init(DEMO_QSCI, &config);

    /* Send g_tipString out. */
    QSCI_WriteBlocking(DEMO_QSCI, g_tipString, sizeof(g_tipString) / sizeof(g_tipString[0]));

    /* Enable RX interrupt. */
    QSCI_EnableInterrupts(DEMO_QSCI, kQSCI_RxFullInterruptEnable);

    while (1)
    {
        /* Send data only when QSCI TX register is empty and ring buffer has data to send out. */
        while (kQSCI_TxDataRegEmptyFlag & QSCI_GetStatusFlags(DEMO_QSCI))
        {
            tmprxIndex = rxIndex;
            tmptxIndex = txIndex;
            if (tmprxIndex != tmptxIndex)
            {
                QSCI_WriteByte(DEMO_QSCI, demoRingBuffer[txIndex]);
                txIndex++;
                txIndex %= DEMO_RING_BUFFER_SIZE;
            }
        }
    }
}
