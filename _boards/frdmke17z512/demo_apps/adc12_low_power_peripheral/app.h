/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "peripherals.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define DEMO_ADC12_CHANNEL_GROUP 0U

#define LED1_INIT() LED_RED_INIT(LOGIC_LED_OFF)
#define LED1_ON()   LED_RED_ON()
#define LED1_OFF()  LED_RED_OFF()

#define LED2_INIT() LED_GREEN_INIT(LOGIC_LED_OFF)
#define LED2_ON()   LED_GREEN_ON()
#define LED2_OFF()  LED_GREEN_OFF()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_ConfigTriggerSource(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
