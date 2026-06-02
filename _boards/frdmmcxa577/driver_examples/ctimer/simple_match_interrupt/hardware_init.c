/*
 * Copyright 2026 NXP
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
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitCTIMERPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_AttachClk(kFRO_HF_to_CTIMER2);
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER2, 1u);
}
/*${function:end}*/
