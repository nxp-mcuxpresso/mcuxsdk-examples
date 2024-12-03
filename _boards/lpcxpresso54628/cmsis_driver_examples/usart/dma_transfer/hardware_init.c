/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    USART0_InitPins();
    BOARD_BootClockPLL220M();

    DMA_Init(DMA0);
}
uint32_t USART0_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(0);
}
uint32_t USART1_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(1);
}
uint32_t USART2_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(2);
}
uint32_t USART3_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(3);
}
uint32_t USART4_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(4);
}
uint32_t USART5_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(5);
}
uint32_t USART6_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(6);
}
uint32_t USART7_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(7);
}
uint32_t USART8_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(8);
}
uint32_t USART9_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(9);
}
/*${function:end}*/
