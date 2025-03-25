/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitLEDsPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void Led_Init(void)
{
    LED_1_INIT();
    LED_2_INIT();
    LED_3_INIT();
}
/*${function:end}*/
