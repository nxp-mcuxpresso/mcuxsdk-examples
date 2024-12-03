/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_CODE_START_NS 0x00020000


#define DEMO_SW1_GPIO      BOARD_SW2_GPIO
#define DEMO_SW1_GPIO_PIN  BOARD_SW2_GPIO_PIN

#define DEMO_SW2_GPIO           BOARD_SW3_GPIO
#define DEMO_SW2_GPIO_PIN       BOARD_SW3_GPIO_PIN
#define DEMO_SW2_GPIO_NSE_MASK  GPIO_PCNS_NSE6_MASK

#define DEMO_LED_RED_GPIO           BOARD_LED_RED_GPIO
#define DEMO_LED_RED_GPIO_PIN       BOARD_LED_RED_GPIO_PIN
#define DEMO_LED_RED_GPIO_NSE_MASK  GPIO_PCNS_NSE4_MASK


#define DEMO_SYSTICK_CLK_FREQ CLOCK_GetFreq(kCLOCK_SystickClk0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
