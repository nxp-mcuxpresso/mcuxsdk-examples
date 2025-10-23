/*
 * Copyright 2025 NXP
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
    BOARD_InitBootClocks();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    BOARD_InitCtimerPwmPins();

    /* CTIMER2 used by this example won't work without CTIMER1 released from reset and clocked */

    /* Enable clock */
    CLOCK_EnableClock(kCLOCK_GateCTIMER1);

    /* Release peripheral reset */
    RESET_ClearPeripheralReset(kCTIMER1_RST_SHIFT_RSTn);
}
/*${function:end}*/