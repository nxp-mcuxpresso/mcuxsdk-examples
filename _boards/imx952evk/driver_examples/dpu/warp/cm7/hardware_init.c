/*
 * Copyright 2025 NXP
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
        .rate = 133333333, /* Source clock value 400Mhz, the current freq 133Mhz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    clk_t dispaxiCLKCfg = {
        .clkId = kCLOCK_dispaxi,
        .pclkId = kCLOCK_syspll1dfs1,
        .rate = 800000000UL, /* AXI clock 800Mhz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    clk_t dispocramCLKCfg = {
        .clkId = kCLOCK_dispocram,
        .pclkId = kCLOCK_syspll1dfs2,
        .rate = 333333333, /* Ocram clocck value 333Mhz*/
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

    clk_t lpi2cClkCfg = {
        .clkId = kCLOCK_lpi2c2,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL, /* 24Mhz for lpi2c */
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
    BOARD_InitI2C2Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRate(&dispapbCLKCfg);
    CLOCK_EnableClock(dispapbCLKCfg.clkId);
    CLOCK_SetRate(&dispaxiCLKCfg);
    CLOCK_EnableClock(dispaxiCLKCfg.clkId);
    CLOCK_SetRate(&dispocramCLKCfg);
    CLOCK_EnableClock(dispocramCLKCfg.clkId);
    CLOCK_SetRate(&lpi2cClkCfg);
    CLOCK_EnableClock(lpi2cClkCfg.clkId);

    BOARD_PrepareDisplay();
}
/*${function:end}*/
