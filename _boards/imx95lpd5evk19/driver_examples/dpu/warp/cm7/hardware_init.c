/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "hal_power.h"
#include "hal_clock.h"
#include "dpu_example.h"
#include "sm_platform.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_dispapbCLKCfg = {
        .clk_id = hal_clock_dispapb,
        .pclk_id = hal_clock_syspll1dfs1div2,
        .div = 3, /* Source clock value 400Mhz, the current freq 133Mhz */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_dispaxiCLKCfg = {
        .clk_id = hal_clock_dispaxi,
        .pclk_id = hal_clock_syspll1dfs2,
        .div = 1, /* AXI clock 800Mhz */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_dispocramCLKCfg = {
        .clk_id = hal_clock_dispocram,
        .pclk_id = hal_clock_syspll1dfs2,
        .div = 2, /* Ocram clocck value 333Mhz*/
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_pwr_s_t pwrst = {
        .did = HAL_POWER_PLATFORM_MIX_SLICE_IDX_DISPLAY,
        .st = hal_power_state_on,
    };

    hal_pwr_s_t campwrst = {
        .did = HAL_POWER_PLATFORM_MIX_SLICE_IDX_CAMERA,
        .st = hal_power_state_on,
    };

    hal_clk_t hal_lpi2cClkCfg = {
        .clk_id = hal_clock_lpi2c2,
        .pclk_id = hal_clock_osc24m,
        .div = 1, /* 24Mhz for lpi2c */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* clang-format on */

    SM_Platform_Init();

    /* Power on the Displaymix */
    HAL_PowerSetState(&pwrst);
    while (HAL_PowerGetState(&pwrst));

    HAL_PowerSetState(&campwrst);
    while (HAL_PowerGetState(&campwrst));

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRootClk(&hal_dispapbCLKCfg);
    HAL_ClockSetRootClk(&hal_dispaxiCLKCfg);
    HAL_ClockSetRootClk(&hal_dispocramCLKCfg);
    HAL_ClockSetRootClk(&hal_lpi2cClkCfg);

    BOARD_PrepareDisplay();
}
/*${function:end}*/
