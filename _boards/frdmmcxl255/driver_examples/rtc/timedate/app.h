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
#define RTC            AON__RTC_AON
#define CGU            AON__CGU
#define SECURITY_RESET kAonSecurity_RST_SHIFT_RSTn
/* Watchdog shall be feed within 2s */
#define WATCHDOG_COUNTER_VALUE 200U
/* RTC delay time us */
#define RTC_DELAY_TIME_US 20000U
/* Delay time to skip tamper button long press */
#define TAMPER_INTERRUPT_DELAY_TIME_US 100000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void APP_InitROSC(void);
void BOARD_Init96MClocksBoot(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
