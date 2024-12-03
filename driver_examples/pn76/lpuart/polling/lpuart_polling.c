/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_lpuart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFF_LENGTH 512

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t data[BUFF_LENGTH];
uint8_t g_tipString[] = "LPUART polling example started\r\nPlease input and there will be an echo\r\n";

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    lpuart_config_t config;
    uint32_t rxDataLen = 0U;

    BOARD_InitHardware();

    config.dW         = 0x000F0000UL;
    config.b.BaudRate = kLPUART_BaudRate115200;

    memset(data, 0, sizeof(data));

    /* INitialize LPUART with source clock as 90 MHZ. */
    LPUART_Init(EXAMPLE_LPUART_BASEADDR, &config, DEMO_LPUART_CLOCK_FREQ);

    LPUART_PollTransmit(EXAMPLE_LPUART_BASEADDR, g_tipString, sizeof(g_tipString) - 1U);

    while (1)
    {
        /* Read data. */
        if (kStatus_Success != LPUART_PollReceive(EXAMPLE_LPUART_BASEADDR, data, &rxDataLen))
        {
            continue;
        }

        /* Send back received data. */
        LPUART_PollTransmit(EXAMPLE_LPUART_BASEADDR, data, rxDataLen);
    }
}
