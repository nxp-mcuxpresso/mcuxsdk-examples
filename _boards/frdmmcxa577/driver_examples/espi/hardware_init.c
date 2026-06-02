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
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitESPIPins();

    CLOCK_AttachClk(kFRO_HF_to_ESPI);
    CLOCK_SetClockDiv(kCLOCK_DivESPI0, 1U);
}
/*${function:end}*/
