/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_UART                 UART1
#define EXAMPLE_UART_CLKSRC          kCLOCK_BusClk
#define EXAMPLE_UART_CLK_FREQ        CLOCK_GetFreq(kCLOCK_BusClk)
#define UART_TX_DMA_CHANNEL          0U
#define UART_RX_DMA_CHANNEL          1U
#define EXAMPLE_UART_DMAMUX_BASEADDR DMAMUX
#define EXAMPLE_UART_DMA_BASEADDR    DMA0
#define UART_TX_DMA_REQUEST          kDmaRequestMux0UART1Tx
#define UART_RX_DMA_REQUEST          kDmaRequestMux0UART1Rx

#define DEMO_UART_IRQn       UART1_IRQn
#define DEMO_UART_IRQHandler UART1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
