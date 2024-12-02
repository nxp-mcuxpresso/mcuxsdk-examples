/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach 12 MHz clock to FLEXCOMM9 (I2C master) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM9);

    /* attach 12 MHz clock to FLEXCOMM8 (I2C slave) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM8);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC9_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kFC8_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_BootClockPLL220M();
    BOARD_InitDebugConsole();
}

uint32_t I2C8_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(8);
}

uint32_t I2C9_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(9);
}

/*${function:end}*/
