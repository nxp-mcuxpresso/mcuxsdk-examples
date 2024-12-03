/*
 * Copyright 2024 NXP
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
/* UART instance and clock */
/* UART2: PTD2,PTD3 used which is J1-5,J1-7 on FRDM-MCXC242 */
#define EXAMPLE_UART                 UART2
#define EXAMPLE_UART_CLKSRC          UART2_CLK_SRC
#define EXAMPLE_UART_CLK_FREQ        CLOCK_GetFreq(UART2_CLK_SRC)
#define UART_TX_DMA_CHANNEL          0U
#define UART_RX_DMA_CHANNEL          1U
#define EXAMPLE_UART_DMAMUX_BASEADDR DMAMUX0
#define EXAMPLE_UART_DMA_BASEADDR    DMA0
#define UART_TX_DMA_REQUEST          kDmaRequestMux0UART2Tx
#define UART_RX_DMA_REQUEST          kDmaRequestMux0UART2Rx

#define DEMO_UART_IRQn       UART2_FLEXIO_IRQn
#define DEMO_UART_IRQHandler UART2_FLEXIO_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
