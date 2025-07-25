/*
 * Copyright 2017,2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_SW_GPIO        BOARD_SW2_GPIO
#define BOARD_SW_PORT        BOARD_SW2_PORT
#define BOARD_SW_GPIO_PIN    BOARD_SW2_GPIO_PIN
#define BOARD_SW_IRQ         INTMUX1_0_IRQn
#define BOARD_SW_IRQ_HANDLER BOARD_SW2_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_SW2_NAME

#define BOARD_LPTMR_TIMER_INSTANCE LPTMR2
#define BOARD_LPTMR_TIMER_IRQ      LPTMR2_IRQn
#define BOARD_LPTMR_CLOCK_SOURCE   kLPTMR_PrescalerClock_1
#define BOARD_TIMER_IRQ_HANDLER    LPTMR2_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
