/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_UTICK_WAKEUP_FROM_SLEEP_MODE 1
#define APP_LED_INIT                     LED_BLUE_INIT(1);
#define APP_LED_TOGGLE                   (LED_BLUE_TOGGLE());
#define APP_INTERNAL_IRC                 BOARD_BootClockFRO12M
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
