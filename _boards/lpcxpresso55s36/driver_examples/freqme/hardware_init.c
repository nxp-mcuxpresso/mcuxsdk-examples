/*
 * Copyright 2021 NXP
 * All rights reserved.
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
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockPLL150M();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Freqme);

    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_FRO12MHZ_FREQM_ENA_MASK | SYSCON_CLOCK_CTRL_FRO_HF_FREQM_ENA_MASK |
                          SYSCON_CLOCK_CTRL_XTAL32MHZ_FREQM_ENA_MASK | SYSCON_CLOCK_CTRL_FRO1MHZ_UTICK_ENA_MASK |
                          SYSCON_CLOCK_CTRL_CLKIN_ENA_MASK;
}
/*${function:end}*/
