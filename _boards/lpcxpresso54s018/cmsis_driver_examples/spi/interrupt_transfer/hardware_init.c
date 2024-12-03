/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach 12 MHz clock to SPI2 */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    /* attach 12 MHz clock to SPI9 */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM9);

    /* reset FLEXCOMM for SPI */
    RESET_PeripheralReset(kFC9_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kFC2_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_BootClockPLL180M();
    BOARD_InitDebugConsole();
}

uint32_t SPI2_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(2U);
}

uint32_t SPI9_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(9U);
}
/*${function:end}*/
