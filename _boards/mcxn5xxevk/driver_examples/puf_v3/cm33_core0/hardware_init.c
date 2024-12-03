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
void ITRC_CDOG_EnableIRQ(void)
{
    /* Set ITRC to reset values after leaving Boot ROM */
    ITRC0->OUT_SEL[3][0] = 0xAAAAAAAA;
    ITRC0->OUT_SEL[4][0] = 0xAAAAAAAA;
}

void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    ITRC_CDOG_EnableIRQ();
}
/*${function:end}*/
