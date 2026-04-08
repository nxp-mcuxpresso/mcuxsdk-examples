/*
 * Copyright 2021-2023, 2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "mcmgr.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    SystemCoreClock = CLOCK_GetRootClockFreq(kCLOCK_Root_M4);

    /* Enable entry to thread mode when unaligned accesses happens */
    SCB->CCR |= (SCB_CCR_UNALIGN_TRP_Msk);
    __DSB();
    __ISB();
}

/*${function:end}*/
