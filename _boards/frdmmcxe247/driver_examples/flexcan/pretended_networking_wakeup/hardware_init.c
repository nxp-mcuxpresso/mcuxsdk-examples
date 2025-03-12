/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_smc.h"
#include "fsl_pmc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Allow SMC all power modes. */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitCANPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(kCLOCK_Can0, kCLOCK_IpSrcSysOscAsync);
}

void APP_SetLowerPowerConfig(void)
{
    /* Default RUN configuration can be used */
}

void APP_EnterLowerPowerMode(void)
{
    SMC_SetPowerModeStop(SMC, kSMC_PartialStop1);
}
/*${function:end}*/
