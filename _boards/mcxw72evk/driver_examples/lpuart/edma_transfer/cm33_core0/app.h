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
#define DEMO_LPUART                 LPUART1
#define DEMO_LPUART_CLK_FREQ        BOARD_DEBUG_UART_CLK_FREQ
#define LPUART_TX_DMA_CHANNEL       0U
#define LPUART_RX_DMA_CHANNEL       1U
#define EXAMPLE_LPUART_DMA_BASEADDR DMA0
#define DEMO_LPUART_TX_EDMA_CHANNEL 34U
#define DEMO_LPUART_RX_EDMA_CHANNEL 33U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
