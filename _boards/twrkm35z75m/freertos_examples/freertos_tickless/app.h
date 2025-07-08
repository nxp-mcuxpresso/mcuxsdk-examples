/*
 * Copyright 2019-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_SW_GPIO        BOARD_SW1_GPIO
#define BOARD_SW_PORT        BOARD_SW1_PORT
#define BOARD_SW_GPIO_PIN    BOARD_SW1_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_SW1_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW1_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_SW1_NAME

#define BOARD_LPTMR_TIMER_INSTANCE   LPTMR0
#define BOARD_LPTMR_TIMER_IRQ        LPTMR0_LPTMR1_IRQn
#define BOARD_LPTMR_CLOCK_SOURCE     kLPTMR_PrescalerClock_1
#define BOARD_LPTMR_BYPASS_PRESCALER true
#define BOARD_TIMER_IRQ_HANDLER      LPTMR0_LPTMR1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
