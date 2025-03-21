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
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP1);
    CLOCK_EnableClock(kCLOCK_GatePORT0);

    /* Release peripheral RESET */
    RESET_PeripheralReset(kPORT0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kPORT1_RST_SHIFT_RSTn);

    CLOCK_SetClockDiv(kCLOCK_DivCTIMER0, 1u);
    CLOCK_AttachClk(kFRO12M_to_CTIMER0);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
