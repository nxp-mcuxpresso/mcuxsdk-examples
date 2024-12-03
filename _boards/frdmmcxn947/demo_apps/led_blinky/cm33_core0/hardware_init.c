/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "peripherals.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_EnableClock(kCLOCK_Gpio0);
    BOARD_InitPins();
    BOARD_BootClockFRO12M();
    /* Initialize the systick module. */
    SysTick_Config(12000000UL);
    LED_RED_INIT(LOGIC_LED_OFF);
}
/*${function:end}*/
