/*
 * Copyright 2025 NXP
 * All rights reserved.
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
    BOARD_InitBootPins();

    /* Bootloader uses default clock setup

       It was observed that calling BOARD_InitBootClocks() in the bootloader and then again
       in the app causes a reboot. This is because it creates an incompatible clock configuration sequence.

     */

    BOARD_InitDebugConsole();
}
/*${function:end}*/
