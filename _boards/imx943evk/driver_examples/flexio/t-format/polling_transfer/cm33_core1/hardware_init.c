/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "sm_platform.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_dfsDiv2ClkCfg = {
        .clk_id = DEMO_DFS_DIV2_CLOCK,
        .pclk_id = hal_clock_osc24m,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 240000000UL, //240MHz
    };
    hal_clk_t hal_flexioClkCfg = {
        .clk_id = DEMO_FLEXIO_CLOCK,
        .pclk_id = DEMO_DFS_DIV2_CLOCK,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 80000000UL, //80MHz
    };
    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRate(&hal_dfsDiv2ClkCfg);
    HAL_ClockEnable(&hal_dfsDiv2ClkCfg);
    HAL_ClockSetRate(&hal_flexioClkCfg);
    HAL_ClockEnable(&hal_flexioClkCfg);
}
/*${function:end}*/
