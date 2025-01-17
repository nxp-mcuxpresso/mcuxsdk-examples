/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define EXAMPLE_LPUART                 LPUART1
#define EXAMPLE_LPUART_CLKSRC          kCLOCK_ScgSysOscClk
#define EXAMPLE_LPUART_CLK_FREQ        CLOCK_GetFreq(kCLOCK_ScgSysOscClk)
#define LPUART_TX_DMA_CHANNEL          0U
#define LPUART_RX_DMA_CHANNEL          1U
#define EXAMPLE_LPUART_DMAMUX_BASEADDR DMAMUX
#define EXAMPLE_LPUART_DMA_BASEADDR    DMA0
#define LPUART_TX_DMA_REQUEST          kDmaRequestMux0LPUART1Tx
#define LPUART_RX_DMA_REQUEST          kDmaRequestMux0LPUART1Rx
#define EXAMPLE_LPUART_IRQHandler      LPUART1_IRQHandler
#define EXAMPLE_LPUART_IRQn            LPUART1_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
