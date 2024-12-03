/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DemoShowResetReason(void);
void DemoShowBootMode(void);
void DemoShowBootloaderBasedOnResetType(void);
void DemoModeSwitch(char ch);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */

int main(void)
{
    char ch;
    uint16_t rstReason;

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("\r\n");

    /* Show reset reason */
    DemoShowResetReason();

    /* Show boot type (bootloader or flash) */
    DemoShowBootMode();

    /* Show how to enter bootloader based on reset type */
    DemoShowBootloaderBasedOnResetType();

    PRINTF("Usage:\r\n");
    PRINTF("  - Input 'f' to switch fast mode without bootloader involved!\r\n");
    PRINTF("  - Input 'F' to switch fast mode with bootloader involved!\r\n");
    PRINTF("  - Input 'n' to switch normal mode without bootloader involved!\r\n");
    PRINTF("  - Input 'N' to switch normal mode with bootloader involved!\r\n");
    PRINTF("  - Input 'b' to software reset without bootloader involved!\r\n");
    PRINTF("  - Input 'B' to software reset with bootloader involved!\r\n");

    while (1)
    {
        ch = GETCHAR();
        DemoModeSwitch(ch);
    }
}
