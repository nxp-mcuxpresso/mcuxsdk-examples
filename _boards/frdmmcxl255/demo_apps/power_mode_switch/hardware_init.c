/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "fsl_wuu.h"
#include "fsl_power.h"
#include "fsl_cmc.h"
/*${header:end}*/

/*${function:start}*/
void WUU0_IRQHandler(void)
{
    uint32_t externalPinFlag;
    externalPinFlag = WUU_GetExternalWakeUpPinsFlag(WUU0);

    if ((externalPinFlag & (1UL << 7UL)) != 0UL)
    {
        WUU_ClearExternalWakeUpPinsFlag(WUU0, externalPinFlag);
    }

    Power_DisableWakeupSource(externalPinFlag);
}

void BOARD_InitHardware(void)
{
    CLOCK_EnableClock(kCLOCK_GateLPUART2);
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP1);

    /* Release peripheral RESET */
    RESET_PeripheralReset(kPORT1_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CMC_EnableDebugOperation(CMC, false);
}

/*${function:end}*/
