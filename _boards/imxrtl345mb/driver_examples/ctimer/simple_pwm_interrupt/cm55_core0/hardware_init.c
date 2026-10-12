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
    /* Use 16 MHz clock for the Ctimer2 */
    /* TODO: Placeholder implementation following the pattern of other methods */
    /* CLOCK_AttachClk(kSFRO_to_CTIMER2); */
    BOARD_InitBootClocks();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    BOARD_InitCtimerMatch0Pins();
}
/*${function:end}*/
