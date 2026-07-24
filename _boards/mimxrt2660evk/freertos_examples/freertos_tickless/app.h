/*
 * Copyright 2021-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
#define BOARD_SW_GPIO        BOARD_USER_BUTTON_GPIO
#define BOARD_SW_GPIO_PIN    BOARD_USER_BUTTON_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_USER_BUTTON_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_USER_BUTTON_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_USER_BUTTON_NAME
#else
#if 0
#define BOARD_SW_GPIO        BOARD_USER_BUTTON_6_GPIO
#define BOARD_SW_GPIO_PIN    BOARD_USER_BUTTON_6_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_USER_BUTTON_6_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_USER_BUTTON_6_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_USER_BUTTON_6_NAME
#else
#define BOARD_SW_GPIO        BOARD_USER_BUTTON_GPIO
#define BOARD_SW_GPIO_PIN    BOARD_USER_BUTTON_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_USER_BUTTON_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_USER_BUTTON_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_USER_BUTTON_NAME
#endif
#endif

#define BOARD_LPTMR_CLOCK_SOURCE     kLPTMR_PrescalerClock_0
#define BOARD_LPTMR_TIMER_INSTANCE   VBAT__LPTMR
#define BOARD_LPTMR_TIMER_IRQ        VBAT_LPTMR_IRQn
#define BOARD_TIMER_IRQ_HANDLER      VBAT_LPTMR_IRQHandler
#define BOARD_LPTMR_BYPASS_PRESCALER true
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
