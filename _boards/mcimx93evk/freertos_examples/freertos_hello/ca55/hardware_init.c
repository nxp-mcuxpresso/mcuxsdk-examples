/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "mmu.h"
#include "irq.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Init board cpu and hardware. */
    MMU_init();
    /* Enable GIC before register any interrupt handler */
    GIC_Enable();
    BOARD_InitClock();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
