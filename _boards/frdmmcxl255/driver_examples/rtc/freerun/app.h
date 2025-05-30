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
#define RTC AON__RTC_AON

/* Initial counter value */
#define INITIAL_COUNTER_VALUE 0x0ULL
/* Alarm 0: Trigger at counter value 0xc8, every 2 seconds */
#define ALARM0_COUNTER_VALUE 0xc8ULL
/* Alarm 1: Trigger at counter value 0x1f4, 5th second */
#define ALARM1_COUNTER_VALUE 0x1f4ULL
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
