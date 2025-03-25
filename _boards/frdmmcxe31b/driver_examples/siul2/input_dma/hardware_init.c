/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitEXTIRQPins();
    BOARD_InitBootClocks();

    BOARD_InitDebugConsole();

    /* Need explicitly enable TCD clock. */
    CLOCK_EnableClock(kCLOCK_Tcd16);
}
/*${function:end}*/
