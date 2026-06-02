/*
 * Copyright 2025 NXP
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
    CLOCK_AttachClk(kFRO_LF_DIV_to_FLEXIO0);
    CLOCK_SetClockDiv(kCLOCK_DivFLEXIO0, 1u);

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kFLEXIO0_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
