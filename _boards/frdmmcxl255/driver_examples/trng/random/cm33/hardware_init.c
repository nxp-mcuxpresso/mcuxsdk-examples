/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitSWD_DEBUGPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
