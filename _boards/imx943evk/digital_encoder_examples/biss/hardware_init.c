/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
#include "sm_platform.h"
#include "pin_mux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* BiSS 20MHz */
    hal_clk_t hal_bissClk = {
		.clk_id = hal_clock_biss,
		.pclk_id = hal_clock_syspll1dfs1div2, /* 400 MHz */
		.div = 20,
		.enable_clk = true,
		.clk_round_opt = hal_clk_round_auto,
	};

    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRootClk(&hal_bissClk);
}
/*${function:end}*/