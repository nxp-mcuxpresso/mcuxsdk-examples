/*
 * Copyright 2023 NXP
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
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO HF to FLEXCAN1 */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcan1Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_FLEXCAN1);

    BOARD_InitPins();
    BOARD_BootClockFROHF48M();
    BOARD_InitDebugConsole();
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
    config.main_domain = kCMC_DeepSleepMode;
    config.wake_domain = kCMC_ActiveOrSleepMode;

    SCG0->FIRCCSR |= SCG_FIRCCSR_FIRCSTEN_MASK;

    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(SPC0, true);

    CMC_EnterLowPowerMode(CMC0, &config);
}
/*${function:end}*/
