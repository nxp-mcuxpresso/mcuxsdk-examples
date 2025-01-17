/*
 * Copyright 2021 NXP
 * All rights reserved.
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
#define EXAMPLE_EWM EWM0
#define SW_GPIO     BOARD_SW8_GPIO
#define SW_GPIO_PIN BOARD_SW8_GPIO_PIN
#define SW_NAME     BOARD_SW8_NAME
/* GPIO port input low-logic level when SW is pressed */
#define SW_GPIO_PRESSED_VALUE 0U

#define WDOG_EWM_IRQn       EWM_IRQn
#define WDOG_EWM_IRQHandler EWM_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void gpio_configure(void);
uint32_t is_key_pressed(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
