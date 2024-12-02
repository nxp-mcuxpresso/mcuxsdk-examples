/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "sm_platform.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    SM_Platform_Init();

    /* clang-format on */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    BOARD_LED_RGPIO->PCNS = 0x0;
}
/*${function:end}*/
