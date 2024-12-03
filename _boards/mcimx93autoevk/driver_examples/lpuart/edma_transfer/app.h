/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPUART                 LPUART2
#define LPUART_CLOCK_ROOT           kCLOCK_Root_Lpuart2
#define LPUART_CLOCK_GATE           kCLOCK_Lpuart2
#define DEMO_LPUART_CLK_FREQ        CLOCK_GetIpFreq(LPUART_CLOCK_ROOT)

#define LPUART_RX_DMA_CHANNEL       kDma3RequestMuxLPUART2Rx
#define LPUART_TX_DMA_CHANNEL       kDma3RequestMuxLPUART2Tx
#define DEMO_LPUART_TX_EDMA_CHANNEL (-1)
#define DEMO_LPUART_RX_EDMA_CHANNEL (-1)
#define EXAMPLE_LPUART_DMA_BASEADDR DMA3
#define EXAMPLE_DMA_CLOCK_ROOT      kCLOCK_Root_M33
#define EXAMPLE_DMA_CLOCK_GATE      kCLOCK_Edma1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
