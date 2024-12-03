/*
 * Copyright 2018 NXP
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
#define EXAMPLE_UART_DMA_BASEADDR SDMAARM1
#define DEMO_UART                 UART4
#define DEMO_IRQn                 UART4_IRQn
#define DEMO_UART_CLK_FREQ        BOARD_DEBUG_UART_CLK_FREQ
#define UART_RX_DMA_CHANNEL       1U
#define UART_TX_DMA_CHANNEL       2U
#define UART_RX_DMA_REQUEST       (28)
#define UART_TX_DMA_REQUEST       (29)
#define DEMO_UART_IRQHandler      UART4_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
