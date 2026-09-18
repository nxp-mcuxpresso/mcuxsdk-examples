/*
 * Copyright 2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitFREQMEPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kFREQME_RST_SHIFT_RSTn);

    /* Attach peripheral clock */
    CLOCK_EnableClock(kCLOCK_GateFREQME);
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF, 1u);
}
/*${function:end}*/
