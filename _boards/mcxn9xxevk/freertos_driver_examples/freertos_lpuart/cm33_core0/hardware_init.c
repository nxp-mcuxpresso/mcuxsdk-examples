/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);
    CLOCK_EnableClock(kCLOCK_LPFlexComm4);
    CLOCK_EnableClock(kCLOCK_LPUart4);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 0u, true);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u, false);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
