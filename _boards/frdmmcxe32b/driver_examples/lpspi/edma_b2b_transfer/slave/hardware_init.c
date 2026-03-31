/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    BOARD_InitDebugConsole();
    BOARD_InitSPIPins();

    /* Need explicitly enable TCD */
    CLOCK_EnableClock(kCLOCK_Tcd0);
    CLOCK_EnableClock(kCLOCK_Tcd1);
}
/*${function:end}*/
