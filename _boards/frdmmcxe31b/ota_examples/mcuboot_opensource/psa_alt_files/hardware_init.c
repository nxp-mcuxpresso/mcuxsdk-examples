/*
 * Copyright 2025-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_cache.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();

    L1CACHE_DisableDCache();

    /* Bootloader uses default clock setup

       It was observed that calling BOARD_InitBootClocks() in the bootloader and then again
       in the app causes a reboot. This is because it creates an incompatible clock configuration sequence.

     */

    BOARD_InitDebugConsole();
}
/*${function:end}*/
