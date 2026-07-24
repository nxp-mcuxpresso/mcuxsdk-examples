/*
 * Copyright 2023, 2026 NXP
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
    BOARD_ConfigMPU();
    BOARD_InitBootClocks();
    BOARD_ConfigTRDC();
    BOARD_InitALT_UARTPins();
    BOARD_InitDEBUG_UARTPins();
}
/*${function:end}*/
