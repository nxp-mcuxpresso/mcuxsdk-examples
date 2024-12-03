/*
 * Copyright 2021 NXP
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
#define DEMO_UART            UART1
#define DEMO_UART_CLKSRC     kCLOCK_CoreSysClk
#define DEMO_UART_CLK_FREQ   CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_UART_IRQn       UART0_UART1_IRQn
#define DEMO_UART_IRQHandler UART0_UART1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
