/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "fsl_device_registers.h"
#include "board.h"
#include "app.h"
#include "fsl_lpsci.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t txbuff[]   = "LPSCI polling example\r\nSend back received data\r\n";
uint8_t rxbuff[20] = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t ch;
    lpsci_config_t config;

    BOARD_InitHardware();

    /*
     * config.parityMode = kLPSCI_ParityDisabled;
     * config.stopBitCount = kLPSCI_OneStopBit;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPSCI_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;

    LPSCI_Init(DEMO_LPSCI, &config, DEMO_LPSCI_CLK_FREQ);
    LPSCI_EnableTx(DEMO_LPSCI, true);
    LPSCI_EnableRx(DEMO_LPSCI, true);

    LPSCI_WriteBlocking(DEMO_LPSCI, txbuff, sizeof(txbuff) - 1);

    while (1)
    {
        LPSCI_ReadBlocking(DEMO_LPSCI, &ch, 1);
        LPSCI_WriteBlocking(DEMO_LPSCI, &ch, 1);
    }
}
