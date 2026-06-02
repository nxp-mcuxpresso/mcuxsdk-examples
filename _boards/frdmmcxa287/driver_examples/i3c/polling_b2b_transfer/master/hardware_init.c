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
    BOARD_InitI3CPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_AttachClk(kFRO_LF_DIV_to_I3C0FCLK);
    CLOCK_SetClockDiv(kCLOCK_DivI3C0_FCLK, 1U);
}
/*${function:end}*/
