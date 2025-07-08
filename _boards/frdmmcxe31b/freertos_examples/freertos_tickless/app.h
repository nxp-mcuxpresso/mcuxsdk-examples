/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_GPIO_BASE      SIUL2
#define BOARD_SW_GPIO        BOARD_SW3_GPIO
#define BOARD_SW_GPIO_PIN    13U
#define BOARD_SW_IRQ         SIUL2_1_IRQn
#define BOARD_SW_IRQ_HANDLER SIUL2_1_IRQHandler
#define BOARD_SW_NAME        "USER"

#define BOARD_RTC_TIMER_INSTANCE  RTC
#define BOARD_RTC_TIMER_IRQ       RTC_IRQn
#define BOARD_RTC_CLOCK_SOURCE    kRTC_SIRCClock
#define BOARD_RTC_CLOCK_FFEQUENCY CLOCK_GetFreq(kCLOCK_SircClk)
#define BOARD_TIMER_IRQ_HANDLER   RTC_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
