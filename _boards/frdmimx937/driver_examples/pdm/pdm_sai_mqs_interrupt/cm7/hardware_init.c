/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "fsl_iomuxc.h"
#include "fsl_sai.h"
/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/
void BOARD_MQS_Init(void)
{
    uint32_t mqsDiv = 1;

    mqsDiv = HMCLK_FREQ / (64 * 16000 * 6);
    BOARD_InitMQS(mqsDiv - 1U);
}

void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t audiopll1vcoCLKCfg = {
        .clkId = kCLOCK_audiopll1ctl,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 3932160000,
    };
    clk_t audiopll1CLKCfg = {
        .clkId = kCLOCK_audiopll1,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 393216000,
    };
    clk_t pdmClkCfg = {
        .clkId = kCLOCK_pdm,
        .pclkId = kCLOCK_audiopll1,
        .rate = 196608000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    clk_t saiCLKCfg = {
        .clkId = kCLOCK_sai1,
        .pclkId = kCLOCK_audiopll1, /* select audiopll1out source(393216000 Hz) */
        .rate = 12288000UL, /* output 12288000 Hz */
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    sai_master_clock_t saiMasterCfg = {
        .mclkOutputEnable = true,
     };

    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_InitMQSPins();
    BOARD_InitPDMPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    CLOCK_SetRate(&audiopll1vcoCLKCfg);
    CLOCK_EnableClock(audiopll1vcoCLKCfg.clkId);
    CLOCK_SetRate(&audiopll1CLKCfg);
    CLOCK_EnableClock(audiopll1CLKCfg.clkId);
    CLOCK_SetRate(&pdmClkCfg);
    CLOCK_EnableClock(pdmClkCfg.clkId);
    CLOCK_SetParent(&saiCLKCfg);
    CLOCK_SetRate(&saiCLKCfg);
    CLOCK_EnableClock(saiCLKCfg.clkId);
    BOARD_ConfigMPU();
    /* select MCLK direction(Enable MCLK clock) */
    saiMasterCfg.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;            /* setup source clock for MCLK */
    saiMasterCfg.mclkHz          = saiMasterCfg.mclkSourceClkHz; /* setup target clock of MCLK */
    SAI_SetMasterClockConfig(DEMO_SAI, &saiMasterCfg);
}
/*${function:end}*/
