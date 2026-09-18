/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_LPUART                 LPUART0
#define EXAMPLE_LPUART_CLK_FREQ        BOARD_DEBUG_UART_CLK_FREQ
#define LPUART_TX_DMA_CHANNEL          0U
#define LPUART_RX_DMA_CHANNEL          1U
#define EXAMPLE_LPUART_DMA_BASEADDR    DMA0
#define LPUART_TX_DMA_REQUEST          kDma0RequestLPUART0Tx
#define LPUART_RX_DMA_REQUEST          kDma0RequestLPUART0Rx
#define EXAMPLE_LPUART_IRQHandler      LPUART0_IRQHandler
#define EXAMPLE_LPUART_IRQn            LPUART0_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
