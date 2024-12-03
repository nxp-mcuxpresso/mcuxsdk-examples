/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "sm_platform.h"
#include "pin_mux.h"
#include "hal_clock.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_tstmr_clk = {
        .clk_id = EXAMPLE_TSTMR_CLOCK_ROOT,
	.pclk_id = hal_clock_osc24m,
	.div = 24,
	.enable_clk = true,
	.clk_round_opt = hal_clk_round_auto,
    };
    /* clang-format on */

    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    HAL_ClockSetRootClk(&hal_tstmr_clk);
}
/*${function:end}*/
