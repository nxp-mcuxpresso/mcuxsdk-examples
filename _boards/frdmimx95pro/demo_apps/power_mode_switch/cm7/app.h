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
#define APP_LPTMR2_IRQ_PRIO     (5U)

#define APP_UART			LPUART7
#define APP_UART_IRQ			LPUART7_IRQn
#define APP_UART_IRQ_PRIO		(5U)
#define APP_CPU_PER_LPI_IDX_UART	CPU_PER_LPI_IDX_LPUART7
#define APP_UART_IRQHANDLER		LPUART7_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
