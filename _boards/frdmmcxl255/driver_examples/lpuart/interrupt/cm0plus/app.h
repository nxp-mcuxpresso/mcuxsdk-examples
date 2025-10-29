/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPUART            AON__LPUART0
#define DEMO_LPUART_CLK_FREQ   BOARD_DEBUG_UART_CLK_FREQ
#define DEMO_LPUART_IRQn       LPUART0_AON_IRQn
#define DEMO_LPUART_IRQHandler LPUART0_AON_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/