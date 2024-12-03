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

    /* attach 12 MHz clock to FLEXCOMM4 (I2C master) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);

    /* attach 12 MHz clock to FLEXCOMM5 (I2C slave) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM5);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC4_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kFC5_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_BootClockPLL150M();
    BOARD_InitDebugConsole();
}

uint32_t I2C0_GetFreq(void)
{
    return 12000000;
}

uint32_t I2C1_GetFreq(void)
{
    return 12000000;
}

uint32_t I2C2_GetFreq(void)
{
    return 12000000;
}

uint32_t I2C3_GetFreq(void)
{
    return 12000000;
}

uint32_t I2C4_GetFreq(void)
{
    return 12000000;
}

uint32_t I2C5_GetFreq(void)
{
    return 12000000;
}

uint32_t I2C6_GetFreq(void)
{
    return 12000000;
}

uint32_t I2C7_GetFreq(void)
{
    return 12000000;
}
/*${function:end}*/
