/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_lpuart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFF_LENGTH 10

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t data[BUFF_LENGTH];
uint8_t g_tipString[] = "LPUART interrupt example started\r\nPlease input and there will be an echo\r\n";
volatile uint8_t rxDataLen;
volatile bool txDone;
volatile bool rxDone;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* User callback function for LPUART TX. */
void LPUART_TxCallback(uint8_t *data, uint32_t length, status_t status)
{
    txDone = true;
}

/* User callback function for LPUART RX. */
void LPUART_RxCallback(uint8_t *data, uint32_t length, status_t status)
{
    rxDataLen = length;
    rxDone = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    lpuart_config_t config;

    BOARD_InitHardware();

    config.dW         = 0x000F0000UL;
    config.b.BaudRate = kLPUART_BaudRate115200;

    memset((void *)data, 0, sizeof(data));

    /* INitialize LPUART with source clock as 90 MHZ. */
    LPUART_Init(EXAMPLE_LPUART_BASEADDR, &config, DEMO_LPUART_CLOCK_FREQ);

    txDone = false;
    LPUART_Transmit(EXAMPLE_LPUART_BASEADDR, g_tipString, sizeof(g_tipString) - 1U, &LPUART_TxCallback);
    while (txDone == false);

    while (1)
    {
        /* Enable receive operation and call callback with received data length from ISR after receiving data. */
        rxDone = false;
        LPUART_Receive(EXAMPLE_LPUART_BASEADDR, (uint8_t *)data, sizeof(data), &LPUART_RxCallback);
        /* Wait until data received. */
        while (rxDone == false);

        /* Enable transmit operation and call callback with status, data length from ISR after transmiting data. */
        txDone = false;
        LPUART_Transmit(EXAMPLE_LPUART_BASEADDR, (uint8_t *)data, rxDataLen, &LPUART_TxCallback);
        /* Wait until TX done. */
        while (txDone == false);
    }
}
