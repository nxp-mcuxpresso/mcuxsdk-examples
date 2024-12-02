/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_cmc.h"
#include "fsl_spc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(kCLOCK_Can0, kCLOCK_IpSrcSoscClk);
    CLOCK_SetIpSrcDiv(kCLOCK_Can0, kSCG_SysClkDivBy1);
}
void APP_SetLowerPowerConfig(void)
{
    CMC_EnableDebugOperation(CMC0, true);
    CMC_SetPowerModeProtection(CMC0, kCMC_AllowAllLowPowerModes);
    CMC_LockPowerModeProtectionSetting(CMC0);
}
void APP_EnterLowerPowerMode(void)
{
    cmc_power_domain_config_t config;

    config.clock_mode  = kCMC_GateCorePlatformClock;
    config.main_domain = kCMC_DeepSleepMode;
    config.wake_domain = kCMC_SleepMode;

    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(SPC0, true);
    CMC_EnterLowPowerMode(CMC0, &config);
}
/*${function:end}*/
