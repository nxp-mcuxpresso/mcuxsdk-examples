/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
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
    /* Select the main clock as source clock of USART0 (debug console) */
    CLOCK_Select(BOARD_DEBUG_USART_CLK_ATTACH);

    CLOCK_Select(kI3C_Clk_From_Fro);

    /* I3C FCLK = 60M/4 = 15M */
    CLOCK_SetI3CFClkDiv(4);

    BOARD_InitPins();
    BOARD_BootClockFRO60M();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
