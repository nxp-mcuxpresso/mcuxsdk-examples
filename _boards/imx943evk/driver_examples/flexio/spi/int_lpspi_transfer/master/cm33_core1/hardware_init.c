/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
#include "sm_platform.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    hal_clk_t hal_lpspiClkCfg = {
        .clk_id = DEMO_LPSPI_CLOCK,
        .pclk_id = hal_clock_osc24m,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };

    hal_clk_t hal_flexioClkCfg = {
        .clk_id = DEMO_FLEXIO_CLOCK,
        .pclk_id = hal_clock_osc24m,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };
    /* clang-format on */

    SM_Platform_Init();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRate(&hal_flexioClkCfg);
    HAL_ClockEnable(&hal_flexioClkCfg);
    HAL_ClockSetRate(&hal_lpspiClkCfg);
    HAL_ClockEnable(&hal_lpspiClkCfg);
}
/*${function:end}*/
