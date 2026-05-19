/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define RTC                             AON__RTC_AON
#define CGU                             AON__CGU
#define SECURITY_RESET                  kAonSecurity_RST_SHIFT_RSTn
#define WATCHDOG_COUNTER_VALUE          200U /* Watchdog shall be feed within 2s */
#define RTC_DELAY_TIME_US               20000U /* RTC delay time us */
#define TAMPER_INTERRUPT_DELAY_TIME_US  100000U /* Delay time to skip tamper button long press */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
