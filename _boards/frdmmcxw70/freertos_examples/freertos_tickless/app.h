/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_SW_GPIO        BOARD_SW3_GPIO
#define BOARD_SW_PORT        BOARD_SW3_PORT
#define BOARD_SW_GPIO_PIN    BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_SW3_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_SW3_NAME

#define BOARD_LPTMR_TIMER_INSTANCE   LPTMR0
#define BOARD_LPTMR_TIMER_IRQ        LPTMR0_IRQn
#define BOARD_LPTMR_CLOCK_SOURCE     kLPTMR_PrescalerClock_0
#define BOARD_TIMER_IRQ_HANDLER      LPTMR0_IRQHandler
#define BOARD_LPTMR_BYPASS_PRESCALER true
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
