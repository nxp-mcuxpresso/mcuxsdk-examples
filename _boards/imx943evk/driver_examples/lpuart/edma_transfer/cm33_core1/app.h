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
#define DEMO_LPUART                 LPUART8
#define DEMO_LPUART_INSTANCE_IDX (8U)
#define LPUART_CLOCK_ROOT           hal_clock_lpuart8
#define DEMO_LPUART_CLK_FREQ        HAL_ClockGetRate(LPUART_CLOCK_ROOT)
#define LPUART_TX_DMA_CHANNEL       19U
#define LPUART_RX_DMA_CHANNEL       20U
#define DEMO_LPUART_RX_EDMA_CHANNEL       kDma3RequestMuxLpuart8Rx
#define DEMO_LPUART_TX_EDMA_CHANNEL       kDma3RequestMuxLpuart8Tx
#define EXAMPLE_LPUART_DMA_BASEADDR EDMA3
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
