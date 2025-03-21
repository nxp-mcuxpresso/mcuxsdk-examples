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
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_EnableClock(kCLOCK_GateLPUART2);
    CLOCK_EnableClock(kCLOCK_GateCMP0);
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP1);
    /* attach peripheral clock */
    CLOCK_AttachClk(kFRO12M_to_CMP0);
    CLOCK_SetClockDiv(kCLOCK_DivCMP0_FUNC, 1U);

    /* Release peripheral RESET */
    RESET_PeripheralReset(kPORT1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
