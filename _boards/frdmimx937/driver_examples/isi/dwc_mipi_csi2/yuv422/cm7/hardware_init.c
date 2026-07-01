/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "fsl_power.h"
#include "isi_example.h"
#include "display_support.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* IMX952_CLK_CAM_ISI expected 667000000*/
    clk_t camisiClkCfg = {
        .clkId = kCLOCK_camIsi,
        .pclkId = kCLOCK_syspll1dfs1,
        .rate = 666666666UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* MIPI CAM APB expected 133330000hz */
    clk_t camapbClkCfg = {
        .clkId = kCLOCK_camApb,
        .pclkId = kCLOCK_syspll1dfs1div2,
        .rate = 133333333UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* MIPI DPHY expected 24mhz */
    clk_t mipiphyClkCfg = {
        .clkId = kCLOCK_camPhyCfg,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* MIPI DPHY expected 24mhz */
    clk_t mipiphytestClkCfg = {
        .clkId = kCLOCK_mipiTestByte,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* MIPI DPHY expected 24mhz */
    clk_t mipiphypllbypassClkCfg = {
        .clkId = kCLOCK_mipiPhyPllBypass,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    pwr_s_t pwrst = {
        .did = POWER_MIX_SLICE_IDX_CAMERA,
        .st = SCMI_POWER_DOMAIN_STATE_ON,
    };
    clk_t lpi2c3ClkCfg = {
        .clkId = kCLOCK_lpi2c3,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL, /* 24Mhz for lpi2c */
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    clk_t lpi2c4ClkCfg = {
        .clkId = kCLOCK_lpi2c4,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL, /* 24Mhz for lpi2c */
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* provide CSI_CLK to connected sensor */
    clk_t ccmclk1ClkCfg = {
        .clkId = kCLOCK_ccmcko1,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL, /* 24Mhz for lpi2c */
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    /* clang-format off */
    clk_t dispapbCLKCfg = {
        .clkId = kCLOCK_dispapb,
        .pclkId = kCLOCK_syspll1dfs1div2,
        .rate = 133333333UL, /* Source clock value 400Mhz, the current freq 133Mhz */
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    clk_t dispaxiCLKCfg = {
        .clkId = kCLOCK_dispaxi,
        .pclkId = kCLOCK_syspll1dfs1,
        .rate = 800000000UL, /* AXI clock 800Mhz */
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    clk_t dispocramCLKCfg = {
        .clkId = kCLOCK_dispocram,
        .pclkId = kCLOCK_syspll1dfs2,
        .rate = 333333333UL, /* Ocram clocck value 333Mhz*/
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    pwr_s_t pwrst1 = {
        .did = POWER_MIX_SLICE_IDX_DISPLAY,
        .st = SCMI_POWER_DOMAIN_STATE_ON,
    };
    clk_t lpi2c2ClkCfg = {
        .clkId = kCLOCK_lpi2c2,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL, /* 24Mhz for lpi2c */
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    SystemPlatformInit();
    /* Power on the Cameramix */
    POWER_SetState(&pwrst);
    while (POWER_GetState(&pwrst));
    /* Power on the Displaymix */
    POWER_SetState(&pwrst1);
    while (POWER_GetState(&pwrst1));

    CLOCK_SetRate(&camisiClkCfg);
    CLOCK_EnableClock(camisiClkCfg.clkId);
    CLOCK_SetRate(&camapbClkCfg);
    CLOCK_EnableClock(camapbClkCfg.clkId);
    CLOCK_SetRate(&mipiphyClkCfg);
    CLOCK_EnableClock(mipiphyClkCfg.clkId);
    CLOCK_SetRate(&mipiphytestClkCfg);
    CLOCK_EnableClock(mipiphytestClkCfg.clkId);
    CLOCK_SetRate(&mipiphypllbypassClkCfg);
    CLOCK_EnableClock(mipiphypllbypassClkCfg.clkId);
    CLOCK_SetRate(&lpi2c3ClkCfg);
    CLOCK_EnableClock(lpi2c3ClkCfg.clkId);
    CLOCK_SetRate(&ccmclk1ClkCfg);
    CLOCK_EnableClock(ccmclk1ClkCfg.clkId);
    CLOCK_SetRate(&dispapbCLKCfg);
    CLOCK_EnableClock(dispapbCLKCfg.clkId);
    CLOCK_SetRate(&dispaxiCLKCfg);
    CLOCK_EnableClock(dispaxiCLKCfg.clkId);
    CLOCK_SetRate(&dispocramCLKCfg);
    CLOCK_EnableClock(dispocramCLKCfg.clkId);
    CLOCK_SetRate(&lpi2c2ClkCfg);
    CLOCK_EnableClock(lpi2c2ClkCfg.clkId);
    CLOCK_SetRate(&lpi2c4ClkCfg);
    CLOCK_EnableClock(lpi2c4ClkCfg.clkId);

    BOARD_BootClockRUN();
    BOARD_InitBootPins();
    BOARD_InitCLKOPins();
    BOARD_InitI2C2Pins();
    BOARD_InitI2C3Pins();
    BOARD_InitI2C4Pins();
    BOARD_InitDebugConsole();
    APP_PrepareCamera();
    BOARD_PrepareDisplay();
}
/*${function:end}*/

