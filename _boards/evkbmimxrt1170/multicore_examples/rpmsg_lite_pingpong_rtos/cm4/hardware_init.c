/*
 * Copyright 2019-2023, 2026 NXP
 *
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
    BOARD_ConfigMPU();
    BOARD_InitPins();
    SystemCoreClock = CLOCK_GetRootClockFreq(kCLOCK_Root_M4);
}

/*${function:end}*/
