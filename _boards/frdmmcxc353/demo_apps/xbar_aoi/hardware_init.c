/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    RESET_ReleasePeripheralReset(kPIT0_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kXBAR0_RST_SHIFT_RSTn);

    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
