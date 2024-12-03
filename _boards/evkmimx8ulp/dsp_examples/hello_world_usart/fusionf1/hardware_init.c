/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board_fusionf1.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    CLOCK_SetXtal0Freq(BOARD_XTAL0_CLK_HZ);
    BOARD_InitDebugConsole();
}
/*${function:end}*/
