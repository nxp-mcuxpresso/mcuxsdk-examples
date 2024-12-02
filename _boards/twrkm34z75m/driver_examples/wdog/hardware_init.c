/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
    /* Note: If using no RTC 32K OSC boards, plsease set SIM_SOPT1[OSC32KSEL] to select a valid clock source */
    /* Select LPO for WDOG */
    CLOCK_SetEr32kClock(0x3);
}
/*${function:end}*/
