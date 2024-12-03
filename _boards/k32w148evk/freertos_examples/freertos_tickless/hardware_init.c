/*
 * Copyright 2021 NXP
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
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

LPTMR_Type *vPortGetLptrmBase(void)
{
    return LPTMR0;
}

IRQn_Type vPortGetLptmrIrqn(void)
{
    return LPTMR0_IRQn;
}
/*${function:end}*/
