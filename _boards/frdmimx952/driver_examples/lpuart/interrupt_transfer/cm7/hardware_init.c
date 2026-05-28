/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t clk = {
        .clkId = BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);
}
/*${function:end}*/
