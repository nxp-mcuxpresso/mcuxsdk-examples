/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    SystemPlatformInit();
    /* clang-format on */
    BOARD_InitBootPins();
    BOARD_InitGPIO2Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    BOARD_LED_RGPIO->PCNS = 0x0;
}
/*${function:end}*/
