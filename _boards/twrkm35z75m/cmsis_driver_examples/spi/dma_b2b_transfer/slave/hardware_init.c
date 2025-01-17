/*
 * Copyright 2019 NXP
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
uint32_t SPI0_GetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_CoreSysClk);
}
uint32_t SPI1_GetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_CoreSysClk);
}
uint32_t SPI2_GetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_CoreSysClk);
}
/*${function:end}*/
