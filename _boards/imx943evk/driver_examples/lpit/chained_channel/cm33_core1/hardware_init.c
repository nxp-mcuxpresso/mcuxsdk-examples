/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "sm_platform.h"
#include "pin_mux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    clk_t lpit_clk = {
        .clkId = LPIT_MASTER_CLOCK_ROOT,
	.pclkId = kCLOCK_Syspll1dfs1div2,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 133333333UL,
    };

    /* clang-format on */

    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRate(&lpit_clk);
    CLOCK_EnableClock(lpit_clk.clkId);
}
/*${function:end}*/
