/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
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
    BOARD_InitBootClocks();
    BOARD_InitSWD_DEBUGPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    BOARD_InitQTMRPins();
    RESET_ReleasePeripheralReset(kAonQTMR0_RST_SHIFT_RSTn);
    AON__SYSCON_AON->PINMUXCLKCTRL = SYSCON_AON_PINMUXCLKCTRL_PINMUX_CLK_CTRL(0);
    AON__INPUTMUX1->QTMR1_TMR[0] = 0x1;
}
/*${function:end}*/
