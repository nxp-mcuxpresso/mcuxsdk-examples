/*
 * Copyright 2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_SW2_GPIO      GPIO0_ALIAS /* Use GPIO ALIAS address. */
#define DEMO_SW2_GPIO_PIN  BOARD_LED_BLUE_GPIO_PIN
#define LED_RED_INIT LED_GREEN_INIT
#define LED_RED_OFF LED_GREEN_OFF
#define LED_RED_ON  LED_GREEN_ON
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
