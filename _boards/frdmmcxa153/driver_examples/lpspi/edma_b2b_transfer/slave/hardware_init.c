/*
 * Copyright 2023 NXP
 * All rights reserved.
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
    RESET_PeripheralReset(kDMA_RST_SHIFT_RSTn);

    /* Attach peripheral clock */
    CLOCK_AttachClk(kFRO12M_to_LPSPI1);
    CLOCK_SetClockDiv(kCLOCK_DivLPSPI1, 1u);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
