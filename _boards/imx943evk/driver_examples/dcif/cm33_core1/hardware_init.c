/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "sm_platform.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "dcif_support.h"
#include "hal_clock.h"
#include "hal_power.h"

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    hal_pwr_st_e st = hal_power_state_off;
    /* clang-format off */
    hal_clk_t hal_clk = {
        .clk_id = hal_clock_invalid,
        .pclk_id = hal_clock_osc24m,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };

    hal_clk_t hal_lpi2cCLKCfg = {
        .clk_id = hal_clock_lpi2c3,
        .pclk_id = hal_clock_osc24m,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };

    hal_clk_t hal_dispapbCLKCfg = {
        .clk_id = hal_clock_dispapb,
        .pclk_id = hal_clock_syspll1dfs1div2,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 133333333UL,
    };
    hal_clk_t hal_dispaxiCLKCfg = {
        .clk_id = hal_clock_dispaxi,
        .pclk_id = hal_clock_syspll1dfs2,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 800000000UL,
    };

    hal_pwr_s_t pwrst = {
        .did = HAL_POWER_PLATFORM_MIX_SLICE_IDX_DISPLAY,
        .st = hal_power_state_on,
    };

    /* clang-format on */
    SM_Platform_Init();
    BOARD_ConfigMPU();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_PowerSetState(&pwrst);
    st = HAL_PowerGetState(&pwrst);
    assert(st == hal_power_state_on);

    HAL_ClockSetRate(&hal_clk);
    HAL_ClockEnable(&hal_clk);
    HAL_ClockSetRate(&hal_lpi2cCLKCfg);
    HAL_ClockEnable(&hal_lpi2cCLKCfg);
    HAL_ClockSetRate(&hal_dispapbCLKCfg);
    HAL_ClockEnable(&hal_dispapbCLKCfg);
    HAL_ClockSetRate(&hal_dispaxiCLKCfg);
    HAL_ClockEnable(&hal_dispaxiCLKCfg);
}
/*${function:end}*/
