/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_UART                 UART2
#define EXAMPLE_UART_CLKSRC          BUS_CLK
#define EXAMPLE_UART_CLK_FREQ        CLOCK_GetFreq(BUS_CLK)
#define UART_TX_DMA_CHANNEL          0U
#define UART_RX_DMA_CHANNEL          1U
#define EXAMPLE_UART_DMAMUX_BASEADDR DMAMUX
#define EXAMPLE_UART_DMA_BASEADDR    DMA0
#define UART_TX_DMA_REQUEST          kDmaRequestMux0UART2Tx
#define UART_RX_DMA_REQUEST          kDmaRequestMux0UART2Rx

#define DEMO_UART_IRQn       UART0_UART1_UART2_UART3_IRQn
#define DEMO_UART_IRQHandler UART0_UART1_UART2_UART3_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
