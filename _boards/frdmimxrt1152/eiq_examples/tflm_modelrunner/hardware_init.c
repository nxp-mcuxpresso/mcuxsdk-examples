/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "timer.h"
/*${header:end}*/

/*${function:start}*/
int64_t os_clock_now(){
    return TIMER_GetTimeInUS();
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    TIMER_Init();
}
/*${function:end}*/
