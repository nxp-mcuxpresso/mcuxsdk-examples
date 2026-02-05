/*
 * Copyright 2025 NXP
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

    CLOCK_SetupFRO16KClocking(kCLKE_16K_SYSTEM | kCLKE_16K_COREMAIN);    
    
    RESET_ReleasePeripheralReset(kTRNG0_RST_SHIFT_RSTn);
    
    CLOCK_EnableClock(kCLOCK_GateTRNG0);
}

/*${function:end}*/
