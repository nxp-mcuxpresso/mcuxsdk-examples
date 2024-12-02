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
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach PLL1Clk0 to FLEXCAN0 */
    CLOCK_SetClkDiv(kCLOCK_DivPLL1Clk0, 2U);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcan0Clk, 1U);
    CLOCK_AttachClk(kPLL1_CLK0_to_FLEXCAN0);

    BOARD_InitPins();
    BOARD_BootClockPLL100M();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
