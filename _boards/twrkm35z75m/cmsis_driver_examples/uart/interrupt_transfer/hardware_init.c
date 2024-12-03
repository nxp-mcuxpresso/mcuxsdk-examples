/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_uart.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_BootClockRUN();
}

uint32_t UART0_GetFreq(void)
{
    return CLOCK_GetFreq(UART0_CLK_SRC);
}

uint32_t UART1_GetFreq(void)
{
    return CLOCK_GetFreq(UART1_CLK_SRC);
}

uint32_t UART2_GetFreq(void)
{
    return CLOCK_GetFreq(UART2_CLK_SRC);
}

uint32_t UART3_GetFreq(void)
{
    return CLOCK_GetFreq(UART3_CLK_SRC);
}
/*${function:end}*/
