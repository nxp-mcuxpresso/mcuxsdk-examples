/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "hal_power.h"
#include "sm_platform.h"
#include "isi_example.h"
#include "dpu_example.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* IMX95_CLK_CAM_ISI expected 667000000*/
    hal_clk_t hal_camisiClkCfg = {
        .clk_id = hal_clock_camIsi,
        .pclk_id = hal_clock_syspll1dfs1,
        .div = 6,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* MIPI CAM APB expected 133330000hz */
    hal_clk_t hal_camapbClkCfg = {
        .clk_id = hal_clock_camApb,
        .pclk_id = hal_clock_syspll1dfs1div2,
        .div = 30,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* MIPI DPHY expected 24mhz */
    hal_clk_t hal_mipiphyClkCfg = {
        .clk_id = hal_clock_mipiPhyCfg,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* MIPI DPHY expected 24mhz */
    hal_clk_t hal_mipiphytestClkCfg = {
        .clk_id = hal_clock_mipiTestByte,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* MIPI DPHY expected 24mhz */
    hal_clk_t hal_mipiphypllbypassClkCfg = {
        .clk_id = hal_clock_mipiPhyPllBypass,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_mipiphypllRefClkCfg = {
        .clk_id = hal_clock_mipiPhyPllRef,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_pwr_s_t pwrst = {
        .did = HAL_POWER_PLATFORM_MIX_SLICE_IDX_CAMERA,
        .st = hal_power_state_on,
    };
    hal_clk_t hal_lpi2c3ClkCfg = {
        .clk_id = hal_clock_lpi2c3,
        .pclk_id = hal_clock_osc24m,
        .div = 1, /* 24Mhz for lpi2c */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* provide CSI_CLK to connected sensor */
    hal_clk_t hal_ccmclk1ClkCfg = {
        .clk_id = hal_clock_ccmcko1,
        .pclk_id = hal_clock_osc24m,
        .div = 1, /* 24Mhz for lpi2c */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };

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
    hal_pwr_s_t pwrst1 = {
        .did = HAL_POWER_PLATFORM_MIX_SLICE_IDX_DISPLAY,
        .st = hal_power_state_on,
    };
    hal_clk_t hal_lpi2c2ClkCfg = {
        .clk_id = hal_clock_lpi2c2,
        .pclk_id = hal_clock_osc24m,
        .div = 1, /* 24Mhz for lpi2c */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    SM_Platform_Init();
    /* Power on the Cameramix */
    HAL_PowerSetState(&pwrst);
    while (HAL_PowerGetState(&pwrst));
    /* Power on the Displaymix */
    HAL_PowerSetState(&pwrst1);
    while (HAL_PowerGetState(&pwrst1));

    HAL_ClockSetRootClk(&hal_camisiClkCfg);
    HAL_ClockSetRootClk(&hal_camapbClkCfg);
    HAL_ClockSetRootClk(&hal_mipiphyClkCfg);
    HAL_ClockSetRootClk(&hal_mipiphytestClkCfg);
    HAL_ClockSetRootClk(&hal_mipiphypllbypassClkCfg);
    HAL_ClockSetRootClk(&hal_mipiphypllRefClkCfg);
    HAL_ClockSetRootClk(&hal_lpi2c3ClkCfg);
    HAL_ClockSetRootClk(&hal_ccmclk1ClkCfg);
    HAL_ClockSetRootClk(&hal_dispapbCLKCfg);
    HAL_ClockSetRootClk(&hal_dispaxiCLKCfg);
    HAL_ClockSetRootClk(&hal_dispocramCLKCfg);
    HAL_ClockSetRootClk(&hal_lpi2c2ClkCfg);

    BOARD_BootClockRUN();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    APP_PrepareCamera();
    BOARD_PrepareDisplay();
}
/*${function:end}*/

