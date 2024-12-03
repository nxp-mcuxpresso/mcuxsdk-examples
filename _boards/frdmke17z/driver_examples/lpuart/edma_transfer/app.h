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
#define DEMO_LPUART                    LPUART0
#define DEMO_LPUART_CLKSRC             kCLOCK_ScgSysOscClk
#define DEMO_LPUART_CLK_FREQ           CLOCK_GetFreq(kCLOCK_ScgSysOscClk)
#define LPUART_TX_DMA_CHANNEL          0U
#define LPUART_RX_DMA_CHANNEL          1U
#define EXAMPLE_LPUART_DMAMUX_BASEADDR DMAMUX
#define EXAMPLE_LPUART_DMA_BASEADDR    DMA0
#define LPUART_TX_DMA_REQUEST          kDmaRequestMux0LPUART0Tx
#define LPUART_RX_DMA_REQUEST          kDmaRequestMux0LPUART0Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
