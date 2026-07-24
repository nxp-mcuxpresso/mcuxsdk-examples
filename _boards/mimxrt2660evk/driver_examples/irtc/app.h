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
/* RT2660 exposes the iRTC instance and its interrupt through the VBAT domain. */
#define RTC            VBAT__RTC
#define RTC_IRQn       VBAT_RTC_IRQn
#define RTC_IRQHandler VBAT_RTC_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
