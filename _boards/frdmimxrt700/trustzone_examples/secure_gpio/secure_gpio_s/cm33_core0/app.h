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
#define DEMO_SW1_GPIO      BOARD_SW5_GPIO
#define DEMO_SW1_GPIO_PIN  BOARD_SW5_GPIO_PIN

#define DEMO_SW2_GPIO           BOARD_LED_BLUE_GPIO
#define DEMO_SW2_GPIO_PIN       BOARD_LED_BLUE_GPIO_PIN
#define DEMO_SW2_GPIO_NSE_MASK  GPIO_PCNS_NSE19_MASK

#define DEMO_LED_RED_GPIO           BOARD_LED_GREEN_GPIO
#define DEMO_LED_RED_GPIO_PIN       BOARD_LED_GREEN_GPIO_PIN
#define DEMO_LED_RED_GPIO_NSE_MASK  GPIO_PCNS_NSE13_MASK

#define LED_RED_INIT LED_GREEN_INIT
#define LED_RED_OFF LED_GREEN_OFF

#define DEMO_SYSTICK_CLK_FREQ CLOCK_GetFreq(kCLOCK_SystickClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
