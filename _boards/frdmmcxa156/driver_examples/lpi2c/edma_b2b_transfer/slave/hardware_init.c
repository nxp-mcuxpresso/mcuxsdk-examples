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

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kDMA_RST_SHIFT_RSTn);

    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivLPI2C3, 1u);
    CLOCK_AttachClk(kFRO12M_to_LPI2C3);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
