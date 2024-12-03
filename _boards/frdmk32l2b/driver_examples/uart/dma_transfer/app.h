/*
 * Copyright 2019 NXP
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
#define DEMO_UART                    UART2
#define DEMO_UART_CLKSRC             kCLOCK_BusClk
#define DEMO_UART_CLK_FREQ           CLOCK_GetFreq(kCLOCK_BusClk)
#define UART_TX_DMA_CHANNEL          0U
#define UART_RX_DMA_CHANNEL          1U
#define EXAMPLE_UART_DMAMUX_BASEADDR DMAMUX0
#define EXAMPLE_UART_DMA_BASEADDR    DMA0
#define UART_TX_DMA_REQUEST          kDmaRequestMux0UART2Tx
#define UART_RX_DMA_REQUEST          kDmaRequestMux0UART2Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
