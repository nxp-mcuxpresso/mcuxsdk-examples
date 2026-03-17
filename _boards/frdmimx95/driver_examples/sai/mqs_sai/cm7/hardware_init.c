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
/*${header:end}*/

/*${function:start}*/
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
        .pclk_id = hal_clock_audiopll1, // select audiopll1out source(393216000 Hz)
        .div = 32, // output 12288000 Hz
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
    HAL_ClockEnable(&hal_audiopll1vcoCLKCfg);
    HAL_ClockEnable(&hal_audiopll1CLKCfg);

    HAL_ClockSetRootClk(&hal_saiCLKCfg);

    /* select MCLK direction(Enable MCLK clock) */
    saiMasterCfg.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;            /* setup source clock for MCLK */
    saiMasterCfg.mclkHz          = saiMasterCfg.mclkSourceClkHz; /* setup target clock of MCLK */
    SAI_SetMasterClockConfig(DEMO_SAI, &saiMasterCfg);
}
/*${function:end}*/
