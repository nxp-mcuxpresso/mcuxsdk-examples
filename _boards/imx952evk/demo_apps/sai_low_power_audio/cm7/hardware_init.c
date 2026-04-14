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
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t pdmClkCfg = {
        .clkId = kCLOCK_pdm,
        .pclkId = kCLOCK_audiopll1,
        .rate = 196608000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    clk_t lpi2cCLKCfg = {
        .clkId = kCLOCK_lpi2c4,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    sai_master_clock_t saiMasterCfg = {
        .mclkOutputEnable = true,
     };
    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_InitI2C4Pins();
    BOARD_InitSAIPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRate(&lpi2cCLKCfg);
    CLOCK_EnableClock(lpi2cCLKCfg.clkId);
    CLOCK_SetRate(&pdmClkCfg);
    CLOCK_EnableClock(pdmClkCfg.clkId);

    BOARD_SetAudmixMode(0x0U);

    /* select MCLK direction(Enable MCLK clock) */
    saiMasterCfg.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;            /* setup source clock for MCLK */
    saiMasterCfg.mclkHz          = saiMasterCfg.mclkSourceClkHz; /* setup target clock of MCLK */
    SAI_SetMasterClockConfig(DEMO_SAI, &saiMasterCfg);

    /* copy resource table to destination address(TCM) */
    copyResourceTable();

    APP_SRTM_Init();
    APP_SRTM_StartCommunication();
}
