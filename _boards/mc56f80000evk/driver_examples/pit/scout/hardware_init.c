/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "peripherals.h"
/*${header:end}*/

void LED_INIT(void);
void LED_TOGGLE(void);

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    BOARD_InitDebugConsole();

    SetIRQBasePriority(0);
}
void LED_INIT(void)
{
    LED_YELLOW_INIT(LOGIC_LED_ON);
}
void LED_TOGGLE(void)
{
    LED_YELLOW_TOGGLE();
}
/*${function:end}*/
