/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include "app.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitLEDsPins();
    BOARD_BootClockFRO12M();
    BOARD_InitBootPeripherals();

    LED_RED_INIT(LOGIC_LED_OFF);
}
/*${function:end}*/
