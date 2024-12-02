/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_power.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_iocon.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Select the main clock as source clock of USART0 (debug console) */
    CLOCK_Select(BOARD_DEBUG_USART_CLK_ATTACH);

    /* Enable lp_osc 1MHz */
    POWER_DisablePD(kPDRUNCFG_PD_LPO_OSC);

    /* I3C FCLK 60M/5 = 12M */
    CLOCK_Select(kI3C_Clk_From_Fro);
    CLOCK_SetI3CFClkDiv(5);

    CLOCK_Select(kI3C_TC_Clk_From_LpOsc);
    CLOCK_SetI3CTCClkDiv(1);
    CLOCK_SetI3CSClkDiv(1);

    BOARD_InitPins();
    BOARD_BootClockFRO60M();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
