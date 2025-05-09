/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "sm_platform.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "hal_power.h"

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    hal_pwr_st_e st = hal_power_state_off;
    /* clang-format off */
    clk_t lpi2cCLKCfg = {
        .clkId = kCLOCK_Lpi2c3,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

    clk_t dispapbCLKCfg = {
        .clkId = kCLOCK_Dispapb,
        .pclkId = kCLOCK_Syspll1dfs1div2,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 133333333UL,
    };
    clk_t dispaxiCLKCfg = {
        .clkId = kCLOCK_Dispaxi,
        .pclkId = kCLOCK_Syspll1dfs2,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
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

    CLOCK_SetRate(&lpi2cCLKCfg);
    CLOCK_EnableClock(lpi2cCLKCfg.clkId);
    CLOCK_SetRate(&dispapbCLKCfg);
    CLOCK_EnableClock(dispapbCLKCfg.clkId);
    CLOCK_SetRate(&dispaxiCLKCfg);
    CLOCK_EnableClock(dispaxiCLKCfg.clkId);
}
/*${function:end}*/
