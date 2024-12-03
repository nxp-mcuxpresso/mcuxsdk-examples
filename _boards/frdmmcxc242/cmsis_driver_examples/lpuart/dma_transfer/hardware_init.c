/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_BootClockRUN();

    /* DMAMux init and DMA init */
    DMAMUX_Init(EXAMPLE_USART_DMAMUX_BASEADDR);
    DMA_Init(EXAMPLE_USART_DMA_BASEADDR);

    CLOCK_SetLpuart0Clock(1U);
}

uint32_t LPUART0_GetFreq(void)
{
    return CLOCK_GetPeriphClkFreq();
}

uint32_t LPUART1_GetFreq(void)
{
    return CLOCK_GetPeriphClkFreq();
}

/*${function:end}*/
