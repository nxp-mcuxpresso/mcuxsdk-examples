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
#include "fsl_xbar.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    RESET_ReleasePeripheralReset(kXBAR0_RST_SHIFT_RSTn);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitAOIPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void IO_Configuration(void)
{
}
/*${function:end}*/
