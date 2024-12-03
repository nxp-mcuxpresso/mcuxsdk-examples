/*
 * Copyright 2020 NXP
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
/* Define the LED configuration */
#define DEMO_LED_GPIO     BOARD_LED_RED_GPIO
#define DEMO_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN

/* Define the IRQ handler entry */
#define DEMO_BUTTON_GPIO     BOARD_BUTTON_SW2_GPIO
#define DEMO_BUTTON_GPIO_PIN BOARD_BUTTON_SW2_GPIO_PIN
#define DEMO_BUTTON_IRQn     GPIOF_IRQn
#define DEMO_BUTTON_PRIO     (1U)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
