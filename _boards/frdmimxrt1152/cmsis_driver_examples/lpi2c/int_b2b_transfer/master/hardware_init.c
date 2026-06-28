/*
 * Copyright 2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitI2CPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

uint32_t LPI2C1_GetFreq(void)
{
    return LPI2C_CLOCK_FREQUENCY;
}
/*${function:end}*/
