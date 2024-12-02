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
#define EXAMPLE_LPUART                 LPUART2
#define EXAMPLE_LPUART_CLKSRC          kCLOCK_ScgSysOscClk
#define EXAMPLE_LPUART_CLK_FREQ        CLOCK_GetFreq(kCLOCK_ScgSysOscClk)
#define LPUART_TX_DMA_CHANNEL          0U
#define LPUART_RX_DMA_CHANNEL          1U
#define EXAMPLE_LPUART_DMAMUX_BASEADDR DMAMUX
#define EXAMPLE_LPUART_DMA_BASEADDR    DMA0
#define LPUART_TX_DMA_REQUEST          kDmaRequestMux0LPUART2Tx
#define LPUART_RX_DMA_REQUEST          kDmaRequestMux0LPUART2Rx
#define EXAMPLE_LPUART_IRQHandler      LPUART2_IRQHandler
#define EXAMPLE_LPUART_IRQn            LPUART2_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
