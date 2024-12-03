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
#include "fsl_common.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    hal_clk_t hal_lpit_clk = {
        .clk_id = LPIT_MASTER_CLOCK_ROOT,
	.pclk_id = hal_clock_syspll1dfs1div2,
	.div = 3, /* 400Mhz/3 = 133.33Mhz */
	.enable_clk = true,
	.clk_round_opt = hal_clk_round_auto,
    };

    /* clang-format on */

    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRootClk(&hal_lpit_clk);
}
/*${function:end}*/
