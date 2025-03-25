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
#define DEMO_RTC             RTC
#define DEMO_RTC_IRQn        RTC_IRQn
#define RTC_CLOCK_FFEQUENCY  CLOCK_GetFreq(kCLOCK_SircClk)
#define RTC_CLOCK_SOURCE     kRTC_SIRCClock
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
