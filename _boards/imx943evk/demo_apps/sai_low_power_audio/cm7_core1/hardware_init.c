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
    /* AUDIO PLL and SAI PDM clock be set by linux side when use aplay or record command, Mcore don't need configure this value in this case. */
    clk_t lpi2cCLKCfg = {
        .clkId = LPI2C_MASTER_CLOCK_ROOT,
        .pclkId = kCLOCK_Osc24m,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    sai_master_clock_t saiMasterCfg = {
        .mclkOutputEnable = true,
     };

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitI2C3Pins();

    BOARD_InitI2C6Pins();

    BOARD_InitSAI1Pins();

    BOARD_InitPDMPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRate(&lpi2cCLKCfg);
    CLOCK_EnableClock(lpi2cCLKCfg.clkId);

    /* Select i2c channel to access codec */
    BOARD_MUX_Select(BOARD_PCA9548_I2C3_ID, BOARD_S4_CHAN_IDX);
    
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, CAN_PDM_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, CAN_PDM_SEL);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, MQS_MIC_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C3_S5_21_ID, MQS_MIC_SEL);    

    /* select MCLK direction(Enable MCLK clock) */
    saiMasterCfg.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;            /* setup source clock for MCLK */
    saiMasterCfg.mclkHz          = saiMasterCfg.mclkSourceClkHz; /* setup target clock of MCLK */
    SAI_SetMasterClockConfig(DEMO_SAI, &saiMasterCfg);

    /* copy resource table to destination address(TCM) */
    copyResourceTable();

    APP_SRTM_Init();
    APP_SRTM_StartCommunication();
}

