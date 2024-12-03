/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_dialog7212.h"
#include "app.h"
#include "fsl_sai.h"
/*${header:end}*/

/*${variable:start}*/
da7212_pll_config_t pllConfig = {
    .source         = kDA7212_PLLClkSourceMCLK,
    .refClock_HZ    = 4000000U,
    .outputClock_HZ = kDA7212_PLLOutputClk12288000,
};
da7212_config_t da7212Config = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = 6000000},
    .dacSource    = kDA7212_DACSourceInputStream,
    .slaveAddress = DA7212_ADDRESS,
    .protocol     = kDA7212_BusI2S,
    .format       = {.mclk_HZ = 12288000, .sampleRate = 16000, .bitWidth = 16},
    .pll          = &pllConfig,
    .sysClkSource = kDA7212_SysClkSourcePLL,
    .isMaster     = true,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_DA7212, .codecDevConfig = &da7212Config};

sai_master_clock_t mclkConfig;

static const scg_sosc_config_t s_scgSoscConfig_BOARD_BootClockRUN = {
    .enableMode = kSCG_SoscEnable,
    .workMode   = kSCG_SysOscModeOscLowPower,
    .freq       = 40000000U,
};

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_InitSysOsc(&s_scgSoscConfig_BOARD_BootClockRUN);

    CLOCK_SetIpSrcDiv(kCLOCK_Sai0, 0);
    CLOCK_SetIpSrcDiv(kCLOCK_Lpi2c1, 0);

    CLOCK_SetIpSrc(kCLOCK_Sai0, kCLOCK_IpSrcSoscClk);
    CLOCK_SetIpSrc(kCLOCK_Lpi2c1, kCLOCK_IpSrcFro192M);
    CLOCK_SetIpSrcDiv(kCLOCK_Lpi2c1, kSCG_SysClkDivBy16);
}

void BOARD_MasterClockConfig(void)
{
    mclkConfig.mclkOutputEnable = true, mclkConfig.mclkHz = 4000000U;
    mclkConfig.mclkSourceClkHz = 40000000U;
    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
}

void BOARD_SAI_RXConfig(sai_transceiver_t *config, sai_sync_mode_t sync)
{
    /* nothing need to do here */
}
/*${function:end}*/
