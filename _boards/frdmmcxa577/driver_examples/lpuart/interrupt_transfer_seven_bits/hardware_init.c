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
    BOARD_InitPins();
    BOARD_InitBootClocks();

    /* Attach peripheral clock */
    CLOCK_AttachClk(kPll1ClkDiv_to_LPUART5);
    CLOCK_SetClockDiv(kCLOCK_DivLPUART5, 1u);
}
/*${function:end}*/
