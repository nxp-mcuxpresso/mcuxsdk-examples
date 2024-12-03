/*
 * Copyright 2021 NXP
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
#define DEMO_UART                       UART1
#define DEMO_UART_CLKSRC                kCLOCK_CoreSysClk
#define DEMO_UART_CLK_FREQ              CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define UART_TX_DMA_CHANNEL             1U
#define UART_RX_DMA_CHANNEL             2U
#define UART_TX_DMAMUX_CHANNEL          0U
#define UART_RX_DMAMUX_CHANNEL          0U
#define EXAMPLE_UART_TX_DMAMUX_BASEADDR DMAMUX1
#define EXAMPLE_UART_RX_DMAMUX_BASEADDR DMAMUX2
#define EXAMPLE_UART_DMA_BASEADDR       DMA0
#define UART_TX_DMA_REQUEST             kDmaRequestMux2UART1Tx
#define UART_RX_DMA_REQUEST             kDmaRequestMux2UART1Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
