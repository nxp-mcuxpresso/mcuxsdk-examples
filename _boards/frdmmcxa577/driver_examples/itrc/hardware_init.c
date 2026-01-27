/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void ITRC_CDOG_EnableIRQ(void)
{
    /* Set ITRC to reset values after leaving Boot ROM */
    ITRC0->OUT_SEL[3][0] = 0xAAA8AA2A; 
    ITRC0->OUT_SEL[4][0] = 0xAAA8AA2A;
}

void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    ITRC_CDOG_EnableIRQ();
}
/*${function:end}*/
