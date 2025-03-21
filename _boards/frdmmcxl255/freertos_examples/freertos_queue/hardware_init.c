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
#include "app.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_EnableClock(kCLOCK_GateLPUART2);
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP1);
    /* Release peripheral RESET */
    RESET_PeripheralReset(kPORT1_RST_SHIFT_RSTn);

    BOARD_InitPins();

    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
