/*
 * Copyright 2019 NXP
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
#define DEMO_LPUART_CLKSRC     kCLOCK_ScgSircAsyncDiv3Clk
#define DEMO_LPUART_CLK_FREQ   CLOCK_GetFreq(kCLOCK_ScgSircAsyncDiv3Clk)
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
