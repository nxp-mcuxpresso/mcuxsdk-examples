/*
 * Copyright 2024 NXP
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
    CLOCK_SetLpuart0Clock(1U);
}

uint32_t LPUART0_GetFreq(void)
{
    return CLOCK_GetPeriphClkFreq();
}
/*${function:end}*/
