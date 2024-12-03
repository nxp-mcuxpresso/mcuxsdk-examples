/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_uart_dma.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_BootClockRUN();

    /* DMAMux init and DMA init */
    DMAMUX_Init(EXAMPLE_USART_DMAMUX_BASEADDR);
    DMA_Init(EXAMPLE_USART_DMA_BASEADDR);
}

extern uart_dma_handle_t UART2_DmaHandle;

void UART2_FLEXIO_IRQHandler(void)
{
    UART_TransferDMAHandleIRQ(UART2, &UART2_DmaHandle);
}

uint32_t UART2_GetFreq(void)
{
    return CLOCK_GetFreq(UART2_CLK_SRC);
}
/*${function:end}*/
