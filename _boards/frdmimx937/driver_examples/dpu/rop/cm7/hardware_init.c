/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_power.h"
#include "fsl_clock.h"
#include "display_support.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t dispapbCLKCfg = {
        .clkId = kCLOCK_dispapb,
        .pclkId = kCLOCK_syspll1dfs1div2,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    clk_t dispaxiCLKCfg = {
        .clkId = kCLOCK_dispaxi,
        .pclkId = kCLOCK_syspll1dfs2,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    clk_t dispocramCLKCfg = {
        .clkId = kCLOCK_dispocram,
        .pclkId = kCLOCK_syspll1dfs2,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    pwr_s_t pwrst = {
        .did = 11,
        .st = SCMI_POWER_DOMAIN_STATE_ON,
    };

    pwr_s_t campwrst = {
        .did = 3,
        .st = SCMI_POWER_DOMAIN_STATE_ON,
    };

    clk_t lpi2c2ClkCfg = {
        .clkId = kCLOCK_lpi2c2,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    clk_t lpi2c4ClkCfg = {
        .clkId = kCLOCK_lpi2c4,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* clang-format on */

    SystemPlatformInit();

    /* Power on the Displaymix */
    POWER_SetState(&pwrst);
    while (POWER_GetState(&pwrst));

    POWER_SetState(&campwrst);
    while (POWER_GetState(&campwrst));

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitI2C4Pins();
    BOARD_InitI2C2Pins();
    BOARD_InitXSPIPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetParent(&dispapbCLKCfg);
    CLOCK_SetRate(&dispapbCLKCfg);
    CLOCK_EnableClock(dispapbCLKCfg.clkId);
    CLOCK_SetParent(&dispaxiCLKCfg);
    CLOCK_SetRate(&dispaxiCLKCfg);
    CLOCK_EnableClock(dispaxiCLKCfg.clkId);
    CLOCK_SetParent(&dispocramCLKCfg);
    CLOCK_SetRate(&dispocramCLKCfg);
    CLOCK_EnableClock(dispocramCLKCfg.clkId);
    CLOCK_SetParent(&lpi2c2ClkCfg);
    CLOCK_SetRate(&lpi2c2ClkCfg);
    CLOCK_EnableClock(lpi2c2ClkCfg.clkId);
    CLOCK_SetParent(&lpi2c4ClkCfg);
    CLOCK_SetRate(&lpi2c4ClkCfg);
    CLOCK_EnableClock(lpi2c4ClkCfg.clkId);

    BOARD_PrepareDisplay();
}
/*${function:end}*/
