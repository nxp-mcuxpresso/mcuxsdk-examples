/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();

    /* DMAMux init and DMA init */
#if FSL_FEATURE_DMA_MODULE_CHANNEL != FSL_FEATURE_DMAMUX_MODULE_CHANNEL
    DMAMUX_Init(EXAMPLE_USART_DMAMUX_TX_BASEADDR);
    DMAMUX_Init(EXAMPLE_USART_DMAMUX_RX_BASEADDR);
    DMA_Init(EXAMPLE_USART_DMA_BASEADDR);
#else
    DMAMUX_Init(EXAMPLE_USART_DMAMUX_BASEADDR);
    DMA_Init(EXAMPLE_USART_DMA_BASEADDR);
#endif
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
