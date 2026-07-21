/*
 * Copyright 2022-2023, 2026 NXP
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
    /* enable clock for GPIO */
    CLOCK_EnableClock(kCLOCK_Gpio0);
    BOARD_InitBootPins();
    SystemCoreClock = CLOCK_GetCoreSysClkFreq();
}

/*${function:end}*/
