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

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitSPIPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_AttachClk(kFRO_HF_DIV_to_LPSPI1);
    CLOCK_AttachClk(kFRO_HF_DIV_to_LPSPI2);

    CLOCK_SetClockDiv(kCLOCK_DivLPSPI1, 1u);
    CLOCK_SetClockDiv(kCLOCK_DivLPSPI2, 1u);
}
/*${function:end}*/
