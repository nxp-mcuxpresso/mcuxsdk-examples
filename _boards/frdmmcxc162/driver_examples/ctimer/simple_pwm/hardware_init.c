/*
 * Copyright 2024 NXP
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
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER0, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER0);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitCTIMERPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
