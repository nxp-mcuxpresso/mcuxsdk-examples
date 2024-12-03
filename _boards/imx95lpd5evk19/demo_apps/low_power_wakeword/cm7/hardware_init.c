/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_codec_common.h"
#include "fsl_wm8962.h"
#include "fsl_codec_adapter.h"
#include "fsl_sai.h"
#include "sm_platform.h"
/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/


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
    hal_clk_t hal_lpi2c4CLKCfg = {
        .clk_id = hal_clock_lpi2c4,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_pdmClkCfg = {
        .clk_id = hal_clock_pdm,
        .pclk_id = hal_clock_audiopll1,
        .div = 2,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_saiCLKCfg = {
        .clk_id = hal_clock_sai3,
        .pclk_id = hal_clock_audiopll1,
        .div = 32,
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


    HAL_ClockSetPllClk(&hal_audiopll1vcoCLKCfg);
    HAL_ClockSetPllClk(&hal_audiopll1CLKCfg);
    HAL_ClockSetRootClk(&hal_lpi2c4CLKCfg);
    HAL_ClockSetRootClk(&hal_saiCLKCfg);
    HAL_ClockSetRootClk(&hal_pdmClkCfg);


    /* Select SAI3 signals */
    pcal6408_handle_t handle6408;
    BOARD_InitPCAL6408_I2C4(&handle6408);
    PCAL6408_SetDirection(&handle6408, (1 << BOARD_PCAL6408_SLOT_SAI3_SEL), kPCAL6408_Output);
    PCAL6408_ClearPins(&handle6408, (1 << BOARD_PCAL6408_SLOT_SAI3_SEL));
    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* select MCLK direction(Enable MCLK clock) */
    saiMasterCfg.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;            /* setup source clock for MCLK */
    saiMasterCfg.mclkHz          = saiMasterCfg.mclkSourceClkHz; /* setup target clock of MCLK */
    SAI_SetMasterClockConfig(DEMO_SAI, &saiMasterCfg);
}
/*${function:end}*/
