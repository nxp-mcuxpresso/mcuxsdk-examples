/*
 * Copyright 2023 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "sm_platform.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    /* 250MHz DMA clock */
    hal_clk_t dmaClkCfg = {
        .clk_id = hal_clock_wakeupaxi,
	.pclk_id = hal_clock_syspll1dfs0,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };
    /* clang-format on */

    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRate(&dmaClkCfg);
    HAL_ClockEnable(&dmaClkCfg);
}
/*${function:end}*/
