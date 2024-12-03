/*
 * Copyright 2017 NXP
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
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void BOARD_DebugConsoleIRQRedirect(void)
{
    INTMUX_EnableInterrupt(INTMUX1, 0, BOARD_UART_IRQ);
}
/*${function:end}*/
