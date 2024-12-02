/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "app_srtm.h"
#include "fsl_mu.h"
#include "rsc_table.h"
#include "fsl_sai.h"
#include "sm_platform.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_pdmClkCfg = {
        .clk_id = hal_clock_pdm,
        .pclk_id = hal_clock_audiopll1,
        .div = 2,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_lpi2cCLKCfg = {
        .clk_id = hal_clock_lpi2c4,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    sai_master_clock_t saiMasterCfg = {
        .mclkOutputEnable = true,
     };
    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRootClk(&hal_lpi2cCLKCfg);
    HAL_ClockSetRootClk(&hal_pdmClkCfg);

    /* select MCLK direction(Enable MCLK clock) */
    saiMasterCfg.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;            /* setup source clock for MCLK */
    saiMasterCfg.mclkHz          = saiMasterCfg.mclkSourceClkHz; /* setup target clock of MCLK */
    SAI_SetMasterClockConfig(DEMO_SAI, &saiMasterCfg);

    /* copy resource table to destination address(TCM) */
    copyResourceTable();

    APP_SRTM_Init();
    APP_SRTM_StartCommunication();
}
