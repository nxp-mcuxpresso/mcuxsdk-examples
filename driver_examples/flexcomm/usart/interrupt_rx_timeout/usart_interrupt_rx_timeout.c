/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_usart.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 16U

#define DEMO_RX_TIMEOUT_US 200UL

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_tipString[] =
    "Usart functional API interrupt rx timeout example\r\nBoard receives characters then sends them out\r\nNow please "
    "input:\r\n";

/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == txIndex)
  Ring buffer empty: (rxIndex == txIndex)
*/
uint8_t demoRingBuffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t txIndex;       /* Index of the data to send out. */
volatile uint16_t rxIndex;       /* Index of the memory to save new arrived data. */
volatile uint8_t rxComplete = 0; /* flag of the data rx complete for timeout. */
/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_USART_IRQHandler(void)
{
    uint8_t data;
    uint32_t status = USART_GetStatusFlags(DEMO_USART);
    /* If new data arrived. */
    if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(DEMO_USART))
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
    if (USART_FIFOSTAT_RXTIMEOUT_MASK & status)
    {
        DEMO_USART->FIFOSTAT |= USART_FIFOSTAT_RXTIMEOUT_MASK;
        rxComplete = 1U;
    }
    GPIO_PortToggle(GPIO, BOARD_INITPINS_LED_PORT, 1U << BOARD_INITPINS_LED_PIN);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    usart_config_t config;
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

    BOARD_InitHardware();
    GPIO_PortInit(GPIO, BOARD_INITPINS_LED_PORT);
    GPIO_PinInit(GPIO, BOARD_INITPINS_LED_PORT, BOARD_INITPINS_LED_PIN, &led_config);
    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUSART_ParityDisabled;
     * config.stopBitCount = kUSART_OneStopBit;
     * config.loopback = false;
     * config.enableTxFifo = false;
     * config.enableRxFifo = false;
     */
    USART_GetDefaultConfig(&config);
    config.baudRate_Bps                    = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx                        = true;
    config.enableRx                        = true;
    config.rxWatermark                     = kUSART_RxFifo2;
    config.rxTimeout.enable                = true;
    config.rxTimeout.resetCounterOnEmpty   = true;
    config.rxTimeout.resetCounterOnReceive = true;
    USART_CalcTimeoutConfig(DEMO_RX_TIMEOUT_US, &config.rxTimeout.prescaler, &config.rxTimeout.counter,
                            CLOCK_GetFreq(kCLOCK_BusClk));
    USART_Init(DEMO_USART, &config, DEMO_USART_CLK_FREQ);

    /* Send g_tipString out. */
    USART_WriteBlocking(DEMO_USART, g_tipString, (sizeof(g_tipString) / sizeof(g_tipString[0])) - 1);

    /* Enable RX interrupt. */
    USART_EnableInterrupts(DEMO_USART, kUSART_RxLevelInterruptEnable | kUSART_RxErrorInterruptEnable);
    /* Enable RX timeout interrupt. */
    USART_EnableInterrupts(DEMO_USART, kUSART_RxTimeoutInterruptEnable);
    EnableIRQ(DEMO_USART_IRQn);

    while (1)
    {
        if (1U == rxComplete)
        {
            rxComplete = 0U;
            /* Send data only when USART TX register is empty and ring buffer has data to send out. */
            while ((kUSART_TxFifoNotFullFlag & USART_GetStatusFlags(DEMO_USART)) && (rxIndex != txIndex))
            {
                USART_WriteByte(DEMO_USART, demoRingBuffer[txIndex]);
                txIndex++;
                txIndex %= DEMO_RING_BUFFER_SIZE;
            }
        }
    }
}
