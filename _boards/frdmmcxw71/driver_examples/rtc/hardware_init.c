/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_ccm32k.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    ccm32k_osc_config_t osc32kConfig = {
        .enableInternalCapBank = true,
        .xtalCap               = kCCM32K_OscXtal0pFCap,
        .extalCap              = kCCM32K_OscExtal16pFCap,
        .coarseAdjustment      = kCCM32K_OscCoarseAdjustmentRange0,
    };
    CCM32K_Set32kOscConfig(CCM32K, kCCM32K_Enable32kHzCrystalOsc, &osc32kConfig);
    CCM32K_SelectClockSource(CCM32K, kCCM32K_ClockSourceSelectOsc32k);
}
/*${function:end}*/
