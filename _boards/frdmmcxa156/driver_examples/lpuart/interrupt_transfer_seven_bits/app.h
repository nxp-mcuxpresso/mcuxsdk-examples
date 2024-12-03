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
#define DEMO_LPUART            LPUART1
#define DEMO_LPUART_CLK_FREQ   CLOCK_GetLpuartClkFreq(1)
#define DEMO_LPUART_IRQn       LPUART1_IRQn
#define DEMO_LPUART_IRQHandler LPUART1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
