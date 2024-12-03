/*
 * Copyright 2023 NXP
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
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);

    CLOCK_EnableClock(kCLOCK_GateFREQME);
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1u);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    CLOCK_SetupExtClocking(8000000U);
    BOARD_InitDebugConsole();
}
/*${function:end}*/
