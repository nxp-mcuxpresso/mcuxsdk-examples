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
    /* CTimer functional clock needs to be greater than or equal to SYSTEM_CLK */
    CLOCK_AttachClk(kFRO_HF_to_CTIMER2);
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER2, 10U);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
