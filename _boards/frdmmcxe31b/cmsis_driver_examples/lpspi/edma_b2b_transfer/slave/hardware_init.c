/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    
    /* Need explicitly enable TCD */
    CLOCK_EnableClock(kCLOCK_Tcd0);
    CLOCK_EnableClock(kCLOCK_Tcd1);
}

uint32_t LPSPI1_GetFreq()
{
    return EXAMPLE_LPSPI_CLOCK_FREQ;
}
/*${function:end}*/
