/*
 * Copyright 2023, 2025 NXP
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
#include "sm_platform.h"
/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/
void BOARD_MQS_Init(void)
{
    uint32_t mqsDiv     = 0U;
    uint64_t mqsDivWide = HAL_ClockGetIpFreq(SAI_CLOCK_ROOT) / (64ULL * 48000ULL);

    /*
     * MQS oversample is 64.
     * Divider uses HMCLK (SAI MCLK root) / (64 * sample_rate * 6).
     * Keep consistent with pdm_sai_mqs_interrupt on this board.
     */
    if ((mqsDivWide == 0ULL) || (mqsDivWide > UINT32_MAX))
    {
        return;
    }

    mqsDiv = (uint32_t)mqsDivWide;
    BOARD_InitMQS(mqsDiv - 1U);
}

void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_audiopll1vcoCLKCfg = {
        .clk_id = hal_clock_audiopll1ctl,
        .clk_round_opt = hal_clk_round_auto,
        .ratel = 3932160000,
        .rateu = 0,
    };
    hal_clk_t hal_audiopll1CLKCfg = {
        .clk_id = hal_clock_audiopll1,
        .clk_round_opt = hal_clk_round_auto,
        .ratel = 393216000,
        .rateu = 0,
    };

    hal_clk_t hal_saiCLKCfg = {
        .clk_id = hal_clock_sai1,
        .pclk_id = hal_clock_audiopll1, /* select audiopll1out source(393216000 Hz) */
        .div = 32,                      /* output 12288000 Hz */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    sai_master_clock_t saiMasterCfg = {
        .mclkOutputEnable = true,
    };
    /* clang-format on */

    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    HAL_ClockSetPllClk(&hal_audiopll1vcoCLKCfg);
    HAL_ClockSetPllClk(&hal_audiopll1CLKCfg);
    HAL_ClockSetRootClk(&hal_saiCLKCfg);

    HAL_ClockEnable(&hal_audiopll1vcoCLKCfg);
    HAL_ClockEnable(&hal_audiopll1CLKCfg);

    /* select MCLK direction(Enable MCLK clock) */
    saiMasterCfg.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;            /* setup source clock for MCLK */
    saiMasterCfg.mclkHz          = saiMasterCfg.mclkSourceClkHz; /* setup target clock of MCLK */
    SAI_SetMasterClockConfig(DEMO_SAI, &saiMasterCfg);
}
/*${function:end}*/
