/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPUART                  HSP__LPUART_0
#define DEMO_LPUART_CLK_FREQ         BOARD_DEBUG_UART_CLK_FREQ
#define LPUART_TX_DMA_CHANNEL        0U
#define LPUART_RX_DMA_CHANNEL        1U
#define DEMO_LPUART_TX_EDMA_CHANNEL  kDmaRequestMux1HspLPUART0Tx
#define DEMO_LPUART_RX_EDMA_CHANNEL  kDmaRequestMux1HspLPUART0Rx
#define EXAMPLE_LPUART_DMA_BASEADDR  MAIN__EDMA3
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
