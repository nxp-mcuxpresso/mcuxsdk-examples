/*
 * Copyright 2026 NXP
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
    /* Update SystemCoreClock to reflect the actual PLL frequency configured
     * by the primary core. Without this call the variable stays at the
     * DEFAULT_SYSTEM_CLOCK (48 MHz) power-on value while the PLL runs at
     * 160 MHz, causing SysTick-based timing to run 3.3x too fast. */
    SystemCoreClockUpdate();
}
/*${function:end}*/
