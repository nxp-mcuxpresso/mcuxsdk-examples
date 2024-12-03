/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_intmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    INTMUX_Init(INTMUX0);
    /* Enable WDOG intmux source on INTMUX channel 0 */
    INTMUX_EnableInterrupt(INTMUX0, 0, WDOG0_IRQn);
}
/*${function:end}*/
