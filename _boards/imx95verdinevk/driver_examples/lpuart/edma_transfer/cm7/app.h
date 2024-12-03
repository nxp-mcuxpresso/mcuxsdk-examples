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
/* Get source clock for LPIT driver */
#define DEMO_LPUART                 LPUART3
#define LPUART_CLOCK_ROOT           hal_clock_lpuart3
#define DEMO_LPUART_CLK_FREQ        HAL_ClockGetIpFreq(LPUART_CLOCK_ROOT)
#define LPUART_TX_DMA_CHANNEL       17U
#define LPUART_RX_DMA_CHANNEL       18U
#define DEMO_LPUART_RX_EDMA_CHANNEL       Dma5RequestMuxLPUART3Rx
#define DEMO_LPUART_TX_EDMA_CHANNEL       Dma5RequestMuxLPUART3Tx
#define EXAMPLE_LPUART_DMA_BASEADDR EDMA5_2
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
