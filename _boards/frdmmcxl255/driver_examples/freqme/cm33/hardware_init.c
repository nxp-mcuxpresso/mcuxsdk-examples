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
    /* Enable clock */
    CLOCK_EnableClock(kCLOCK_GateFREQME);

    /* Set clock */
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1U);

    BOARD_InitBootClocks();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    BOARD_InitFREQMEPins();
}
/*${function:end}*/