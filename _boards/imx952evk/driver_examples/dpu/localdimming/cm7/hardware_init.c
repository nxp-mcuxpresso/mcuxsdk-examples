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
#include "ld_support.h"
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

    clk_t lpi2c2ClkCfg = {
        .clkId = kCLOCK_lpi2c2,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL, /* 24Mhz for lpi2c */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    clk_t lpi2c3ClkCfg = {
        .clkId = kCLOCK_lpi2c3,
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
    BOARD_InitCLKOPins();
    BOARD_InitI2C2Pins();
    BOARD_InitI2C3Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRate(&dispapbCLKCfg);
    CLOCK_EnableClock(dispapbCLKCfg.clkId);
    CLOCK_SetRate(&dispaxiCLKCfg);
    CLOCK_EnableClock(dispaxiCLKCfg.clkId);
    CLOCK_SetRate(&dispocramCLKCfg);
    CLOCK_EnableClock(dispocramCLKCfg.clkId);
    CLOCK_SetRate(&lpi2c2ClkCfg);
    CLOCK_EnableClock(lpi2c2ClkCfg.clkId);
    CLOCK_SetRate(&lpi2c3ClkCfg);
    CLOCK_EnableClock(lpi2c3ClkCfg.clkId);

    /* Config OCRAM */
    DISPLAY__OCRAM_MECC->RAMCR = 0U;
    DISPLAY__OCRAM_MECC->RAMIAS = 0x4B300000U;
    DISPLAY__OCRAM_MECC->RAMIAE = 0x4B304000U;
    DISPLAY__OCRAM_MECC->RAMCR = 0x1U;
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    BOARD_LD_Init();

    BOARD_PrepareDisplay();
}
/*${function:end}*/
