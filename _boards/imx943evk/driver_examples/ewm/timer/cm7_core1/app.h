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
#define EXAMPLE_EWM EWM
#define WDOG_EWM_IRQ       EWM_IRQn
#define WDOG_EWM_IRQHandler EWM_IRQHandler
#define EXAMPLE_TIMER_CLK_ID (kCLOCK_Buswakeup)
/* unit: micro second*/
#define EXAMPLE_TIMEOUT_PERIOD_COUNT (1000)
#define EXAMPLE_TIMER_CLK_FREQ (CLOCK_GetRate(EXAMPLE_TIMER_CLK_ID))
#define EXAMPLE_TIMER_INSTANCE (1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
