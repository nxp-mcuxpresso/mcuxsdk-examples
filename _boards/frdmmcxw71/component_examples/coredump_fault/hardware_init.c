/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_cmc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

bool BOARD_SoftwareResetAsserted(void)
{
    if ((CMC_GetSystemResetStatus(CMC0) & (uint32_t)kCMC_SoftwareReset) != 0UL)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*${function:end}*/
