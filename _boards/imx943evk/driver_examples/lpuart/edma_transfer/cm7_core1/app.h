/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Get source clock for LPIT driver */
#define DEMO_LPUART                 LPUART12
#define DEMO_LPUART_INSTANCE_IDX (12U)
#define LPUART_CLOCK_ROOT           kCLOCK_Lpuart12
#define DEMO_LPUART_CLK_FREQ        CLOCK_GetRate(LPUART_CLOCK_ROOT)
#define LPUART_TX_DMA_CHANNEL       0U
#define LPUART_RX_DMA_CHANNEL       1U
#define DEMO_LPUART_RX_EDMA_CHANNEL       kDma4RequestMuxLpuart12Rx
#define DEMO_LPUART_TX_EDMA_CHANNEL       kDma4RequestMuxLpuart12Tx
#define EXAMPLE_LPUART_DMA_BASEADDR EDMA4
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
