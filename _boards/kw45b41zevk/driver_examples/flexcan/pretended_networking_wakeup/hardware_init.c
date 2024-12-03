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
    CLOCK_EnableClockLPMode(kCLOCK_Can0, kCLOCK_IpClkControl_fun3);
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

    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_SleepMode;
    config.wake_domain = kCMC_SleepMode;

    /* Set NBU into deep power down mode */
    RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0xF);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;

    /* Set CM33 power mode */
    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(SPC0, true);
    CMC_EnterLowPowerMode(CMC0, &config);
}

void App_SetSPCConfiguration(void)
{
    /*
     * SPC is configured as follows:
     *
     * KW45 active power mode:
     * DCDC:               DCDC output = 1.8V, DCDC drive strength = normal drive strength
     * LDO SYS:            LDO SYS output = 1.8V, LDO SYS drive strength = normal drive strength
     * LDO CORE:           LDO CORE output = 1.05V, LDO CORE drive strength = normal drive strength
     *
     * KW45 low power mode:
     * DCDC:               DCDC output = 1.25V, DCDC drive strength = low drive strength
     * LDO SYS:            LDO SYS output = 1.8V, LDO SYS drive strength = low drive strength
     * LDO CORE:           LDO CORE output = 1.05V, LDO CORE drive strength = low drive strength
     */

    status_t status;

    /*********************************** SPC active mode configuration ***********************************/
    spc_active_mode_regulators_config_t activeModeRegulatorOption;

    /* Enable LVDs and HVDs in active mode */
    SPC_EnableActiveModeCoreHighVoltageDetect(SPC0, true);
    SPC_EnableActiveModeCoreLowVoltageDetect(SPC0, true);
    SPC_EnableActiveModeSystemHighVoltageDetect(SPC0, true);
    SPC_EnableActiveModeSystemLowVoltageDetect(SPC0, true);
    SPC_EnableActiveModeIOHighVoltageDetect(SPC0, true);
    SPC_EnableActiveModeIOLowVoltageDetect(SPC0, true);

    /* Active mode settings */
    activeModeRegulatorOption.bandgapMode                      = kSPC_BandgapEnabledBufferDisabled;
    activeModeRegulatorOption.lpBuff                           = false;
    activeModeRegulatorOption.DCDCOption.DCDCVoltage           = kSPC_DCDC_SafeModeVoltage;
    activeModeRegulatorOption.DCDCOption.DCDCDriveStrength     = kSPC_DCDC_NormalDriveStrength;
    activeModeRegulatorOption.SysLDOOption.SysLDOVoltage       = kSPC_SysLDO_NormalVoltage;
    activeModeRegulatorOption.SysLDOOption.SysLDODriveStrength = kSPC_SysLDO_NormalDriveStrength;
    activeModeRegulatorOption.CoreLDOOption.CoreLDOVoltage     = kSPC_CoreLDO_MidDriveVoltage;
#if defined(FSL_FEATURE_SPC_HAS_CORELDO_VDD_DS) && FSL_FEATURE_SPC_HAS_CORELDO_VDD_DS
    activeModeRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength;
#endif /* FSL_FEATURE_SPC_HAS_CORELDO_VDD_DS */

    /* Configure active mode settings */
    status = SPC_SetActiveModeRegulatorsConfig(SPC0, &activeModeRegulatorOption);
    if (status != kStatus_Success)
    {
        return;
    }
    while (SPC_GetBusyStatusFlag(SPC0));

    /*********************************** SPC low power mode configuration ***********************************/
    spc_lowpower_mode_regulators_config_t lowPowerRegulatorOption;

    /* Disable LVDs and HVDs in low power mode*/
    SPC_EnableLowPowerModeCoreHighVoltageDetect(SPC0, false);
    SPC_EnableLowPowerModeCoreLowVoltageDetect(SPC0, false);
    SPC_EnableLowPowerModeSystemHighVoltageDetect(SPC0, false);
    SPC_EnableLowPowerModeSystemLowVoltageDetect(SPC0, false);
    SPC_EnableLowPowerModeIOHighVoltageDetect(SPC0, false);
    SPC_EnableLowPowerModeIOLowVoltageDetect(SPC0, false);

    /* Low power mode settings */
    lowPowerRegulatorOption.lpIREF                             = false;
    lowPowerRegulatorOption.bandgapMode                        = kSPC_BandgapDisabled;
    lowPowerRegulatorOption.lpBuff                             = false;
    lowPowerRegulatorOption.CoreIVS                            = false;
    lowPowerRegulatorOption.DCDCOption.DCDCVoltage             = kSPC_DCDC_LowUnderVoltage;
    lowPowerRegulatorOption.DCDCOption.DCDCDriveStrength       = kSPC_DCDC_LowDriveStrength;
    lowPowerRegulatorOption.SysLDOOption.SysLDODriveStrength   = kSPC_SysLDO_LowDriveStrength;
    lowPowerRegulatorOption.CoreLDOOption.CoreLDOVoltage       = kSPC_CoreLDO_MidDriveVoltage;
    lowPowerRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_LowDriveStrength;

    /* Configure low power mode settings */
    status = SPC_SetLowPowerModeRegulatorsConfig(SPC0, &lowPowerRegulatorOption);
    if (status != kStatus_Success)
    {
        return;
    }
    while (SPC_GetBusyStatusFlag(SPC0));

}
/*${function:end}*/
