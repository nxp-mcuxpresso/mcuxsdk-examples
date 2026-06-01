/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "dcif_support.h"
#include "fsl_power.h"

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    uint32_t st = SCMI_POWER_DOMAIN_STATE_OFF;
    /* clang-format off */
    clk_t clk = {
        .clkId = kCLOCK_IpInvalid,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

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

    pwr_s_t pwrst = {
        .did =  POWER_MIX_SLICE_IDX_DISPLAY,
        .st = SCMI_POWER_DOMAIN_STATE_ON,
    };

    /* clang-format on */
    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitI2C3Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    POWER_SetState(&pwrst);
    st = POWER_GetState(&pwrst);
    assert(st == SCMI_POWER_DOMAIN_STATE_ON);
    (void)st;

    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);
    CLOCK_SetRate(&lpi2cCLKCfg);
    CLOCK_EnableClock(lpi2cCLKCfg.clkId);
    CLOCK_SetParent(&dispapbCLKCfg);
    CLOCK_SetRate(&dispapbCLKCfg);
    CLOCK_EnableClock(dispapbCLKCfg.clkId);
    CLOCK_SetParent(&dispaxiCLKCfg);
    CLOCK_SetRate(&dispaxiCLKCfg);
    CLOCK_EnableClock(dispaxiCLKCfg.clkId);
}
/*${function:end}*/
