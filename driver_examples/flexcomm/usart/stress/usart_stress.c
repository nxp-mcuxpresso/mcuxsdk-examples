/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_usart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define RX_RING_BUFFER_SIZE 20U

#define DEMO_RX_BUFFER_SIZE 1U
#define DEMO_PRNG_SEED 12345U

/* Range for main loop to randomly wait between USART calls */
#define DEMO_RNG_MIN_DELAY 100U
#define DEMO_RNG_MAX_DELAY 1000U

/* Set frequency of SysTick interrupts */
#define DEMO_SYSTICK_FREQ 100U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* USART user callback */
void USART_UserCallback(USART_Type *base, usart_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
usart_handle_t g_uartHandle;
uint8_t g_mainString[] = "M_";
uint8_t g_systickString[] = "S_";
uint8_t g_rxRingBuffer[RX_RING_BUFFER_SIZE] = {0}; /* RX ring buffer. */

/* Track the count of USART API calls and how many times API returned busy flag  */
static uint32_t systick_called_count = 0;
static uint32_t systick_rx_catch_count = 0;
static uint32_t systick_txbusy_count = 0;
static uint32_t main_called_count = 0;
static uint32_t main_rx_catch_count = 0;
static uint32_t main_txbusy_count = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Any non-blocking USART transfer can be initiated from interrupt handler */
void SysTick_Handler()
{
    status_t status;

    usart_transfer_t xferTx;
    xferTx.data     = g_systickString;
    xferTx.dataSize = sizeof(g_systickString) - 1;

    usart_transfer_t xferRx;
    static uint8_t rxBuffer[DEMO_RX_BUFFER_SIZE] = {0};
    xferRx.data     = rxBuffer;
    xferRx.dataSize = DEMO_RX_BUFFER_SIZE;
    size_t xferRxBytes;

    /* Call Tx/Rx on odd/even handler calls. This simulates application using
     * USART from timer handler in a predictable pattern. */
    if (systick_called_count++ % 2)
    {
        status = USART_TransferSendNonBlocking(DEMO_USART, &g_uartHandle, &xferTx);
        if (status == kStatus_USART_TxBusy)
            systick_txbusy_count++;
    }
    else
    {
        status = USART_TransferReceiveNonBlocking(DEMO_USART, &g_uartHandle, &xferRx, &xferRxBytes);
        if (status != kStatus_USART_RxBusy)
            systick_rx_catch_count++;
    }
}

/* For the purposes of this example, we use a simple Linear congruential
 * generator for randomness.
 **/
static uint32_t lcg_rand()
{
    static uint32_t seed = DEMO_PRNG_SEED;
    seed = (1103515245 * seed + 12345) % 0xFFFFFFFF;
    return seed;
}

/* Simulate workload with random number of cycles
 **/
void rand_delay(uint32_t min, uint32_t max)
{
    assert(max > min);
    if (max > min) {
        volatile uint32_t cycles = min + lcg_rand() % (max-min+1);
        while(cycles--);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    status_t status;

    BOARD_InitHardware();

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUSART_ParityDisabled;
     * config.stopBitCount = kUSART_OneStopBit;
     * config.loopback = false;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    usart_config_t config;
    USART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    USART_Init(DEMO_USART, &config, DEMO_USART_CLK_FREQ);
    USART_TransferCreateHandle(DEMO_USART, &g_uartHandle, NULL, NULL);
    USART_TransferStartRingBuffer(DEMO_USART, &g_uartHandle, g_rxRingBuffer, RX_RING_BUFFER_SIZE);

    /* Init a source of periodic timed interrupts */
    SysTick_Config(SystemCoreClock/DEMO_SYSTICK_FREQ);

    usart_transfer_t xferTx;
    xferTx.data     = g_mainString;
    xferTx.dataSize = sizeof(g_mainString) - 1;

    usart_transfer_t xferRx;
    static uint8_t rxBuffer[DEMO_RX_BUFFER_SIZE] = {0};
    xferRx.data     = rxBuffer;
    xferRx.dataSize = DEMO_RX_BUFFER_SIZE;
    size_t xferRxBytes;

    /* Call Tx/Rx on odd/even loops. The loop also uses random delay to simulate
     * the app doing some amount of work between USART calls.
     **/
    while(true) {
        rand_delay(DEMO_RNG_MIN_DELAY, DEMO_RNG_MAX_DELAY);
        if (main_called_count++ % 2)
        {
            status = USART_TransferSendNonBlocking(DEMO_USART, &g_uartHandle, &xferTx);
            if (status == kStatus_USART_TxBusy)
                main_txbusy_count++;
        }
        else
        {
            status = USART_TransferReceiveNonBlocking(DEMO_USART, &g_uartHandle, &xferRx, &xferRxBytes);
            if (status != kStatus_USART_RxBusy)
                main_rx_catch_count++;
        }
    }
}
