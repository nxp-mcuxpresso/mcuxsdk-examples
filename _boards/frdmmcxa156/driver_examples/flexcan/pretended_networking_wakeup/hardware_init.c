/*
 * Copyright 2024 NXP
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
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivFLEXCAN0, 1U);
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1U);
    CLOCK_AttachClk(kFRO_HF_DIV_to_FLEXCAN0);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
void APP_SetLowerPowerConfig(void)
{
    /* Disable low power debug. */
    CMC_EnableDebugOperation(CMC, false);
    /* Allow all power mode */
    CMC_SetPowerModeProtection(CMC, kCMC_AllowAllLowPowerModes);

    /* Disable flash memory accesses and place flash memory in low-power state whenever the core clock
       is gated. And an attempt to access the flash memory will cause the flash memory to exit low-power
       state for the duration of the flash memory access. */
    CMC_ConfigFlashMode(CMC, true, true, false);
}
void APP_EnterLowerPowerMode(void)
{
    cmc_power_domain_config_t config;

    config.clock_mode  = kCMC_GateCoreClock;
    config.main_domain = kCMC_ActiveOrSleepMode;

    CMC_EnterLowPowerMode(CMC, &config);
}
/*${function:end}*/
