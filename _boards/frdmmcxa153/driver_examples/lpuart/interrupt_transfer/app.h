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
#define DEMO_LPUART            LPUART0
#define DEMO_LPUART_CLK_FREQ   (BOARD_DEBUG_UART_CLK_FREQ)
#define DEMO_LPUART_IRQn       LPUART0_IRQn
#define DEMO_LPUART_IRQHandler LPUART0_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
