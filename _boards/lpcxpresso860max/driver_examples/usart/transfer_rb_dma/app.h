/*
 * Copyright 2023 NXP
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
#define EXAMPLE_USART              USART0
#define EXAMPLE_USART_CLK_SRC      kCLOCK_MainClk
#define EXAMPLE_USART_CLK_FREQ     CLOCK_GetFreq(EXAMPLE_USART_CLK_SRC)
#define EXAMPLE_USART_DMA_BASEADDR DMA0
#define USART_RX_DMA_CHANNEL       0

#define EXAMPLE_TIMEOUT_PERIOD_MS (10U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
