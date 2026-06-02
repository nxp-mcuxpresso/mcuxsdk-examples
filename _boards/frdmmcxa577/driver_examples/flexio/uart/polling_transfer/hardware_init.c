/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitFLEXIO_UARTPins();
    BOARD_InitBootClocks();

    /* Attach peripheral clock */
    CLOCK_AttachClk(kPll1ClkDiv_to_FLEXIO0);
    CLOCK_SetClockDiv(kCLOCK_DivFLEXIO0, 1u);
}
/*${function:end}*/
