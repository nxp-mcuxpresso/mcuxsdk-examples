/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPUART                    LPUART_5
#define DEMO_LPUART_CLK_FREQ           BOARD_DEBUG_UART_CLK_FREQ
#define LPUART_TX_DMA_CHANNEL          16U
#define LPUART_RX_DMA_CHANNEL          17U
#define LPUART_TX_DMA_REQUEST          kDmaRequestMux1LPUART5LPUART13Tx
#define LPUART_RX_DMA_REQUEST          kDmaRequestMux1LPUART5LPUART13Rx
#define EXAMPLE_LPUART_TX_DMAMUX_BASEADDR DMAMUX_1
#define EXAMPLE_LPUART_RX_DMAMUX_BASEADDR DMAMUX_1
#define EXAMPLE_LPUART_DMA_BASEADDR    EDMA
#define LPUART_TX_DMAMUX_CHANNEL       0U
#define LPUART_RX_DMAMUX_CHANNEL       1U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
