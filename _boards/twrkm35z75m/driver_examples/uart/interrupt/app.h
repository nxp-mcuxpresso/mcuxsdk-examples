/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_UART            UART2
#define DEMO_UART_CLKSRC     BUS_CLK
#define DEMO_UART_CLK_FREQ   CLOCK_GetFreq(BUS_CLK)
#define DEMO_UART_IRQn       UART0_UART1_UART2_UART3_IRQn
#define DEMO_UART_IRQHandler UART0_UART1_UART2_UART3_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
