/*
 * Copyright 2020 NXP
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
    LED_0_INIT(LOGIC_LED_OFF);
    LED_1_INIT(LOGIC_LED_OFF);
    LED_2_INIT(LOGIC_LED_OFF);
    LED_3_INIT(LOGIC_LED_OFF);
    LED_4_INIT(LOGIC_LED_OFF);
    LED_5_INIT(LOGIC_LED_OFF);
    LED_6_INIT(LOGIC_LED_OFF);
    LED_7_INIT(LOGIC_LED_OFF);
    LED_8_INIT(LOGIC_LED_ON);
}
void LED_TOGGLE(void)
{
    LED_8_TOGGLE();
}
/*${function:end}*/
