/*
 * Copyright 2024 NXP
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
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivFLEXIO0, 1u);
    CLOCK_AttachClk(kFRO_LF_DIV_to_FLEXIO0);

    BOARD_InitPins();
    BOARD_InitBootClocks();
}
/*${function:end}*/
