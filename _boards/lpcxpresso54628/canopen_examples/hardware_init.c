/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    BOARD_InitPins();

    BOARD_BootClockPLL160M();
#if USE_CANOPEN_FD
    /* Set MCAN clock 160Mhz/2=80Mhz for CAN FD. */
    CLOCK_SetClkDiv(kCLOCK_DivCan0Clk, 2U, true);
#else
    /* Set MCAN clock 160Mhz/8=20Mhz for legacy CAN. */
    CLOCK_SetClkDiv(kCLOCK_DivCan0Clk, 8U, true);
#endif

    BOARD_InitDebugConsole();
}
/*${function:end}*/
