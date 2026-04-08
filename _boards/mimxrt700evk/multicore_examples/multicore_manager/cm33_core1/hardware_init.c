/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "mcmgr.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitLEDsPins_Sense();
    BOARD_InitDebugConsole();
}

/*${function:end}*/
