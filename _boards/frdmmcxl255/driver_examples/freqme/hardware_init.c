/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include "fsl_aon_common.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    aon_config_t aonConfig;

    CLOCK_EnableClock(kCLOCK_GateLPUART2);
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP1);
    CLOCK_EnableClock(kCLOCK_GateFREQME);

    /* Release peripheral RESET */
    RESET_PeripheralReset(kPORT1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Enable the ROSC */
    Aon_GetDefaultConfig(&aonConfig);
    aonConfig.aonAdvcEnableOptions   = kAON_AdvcEnableOptions_Disable;
    aonConfig.aonAdvcOperationalMode = kAON_AdvcOperationalMode_Disabled;
    Aon_Init(&aonConfig);

    /* Enables the output of the RTC clock towards the main domain. */
    Aon_RegWrite((uint32_t)(&AON_CGU->CLK_CONFIG), CGU_CLK_CONFIG_XTAL32_OUT_EN(1u) | CGU_CLK_CONFIG_FRO3M_EN(1u), 0);
}
/*${function:end}*/
