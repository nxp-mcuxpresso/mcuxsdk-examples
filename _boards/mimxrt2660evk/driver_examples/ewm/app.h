/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define EXAMPLE_EWM         WAKE__EWM
#define WDOG_EWM_IRQn       WAKE_EWM_IRQn
#define WDOG_EWM_IRQHandler WAKE_EWM_IRQHandler

#if 0
#define SW_GPIO               BOARD_USER_BUTTON_6_GPIO
#define SW_GPIO_PIN           BOARD_USER_BUTTON_6_GPIO_PIN
#define SW_NAME               BOARD_USER_BUTTON_6_NAME
/* GPIO port input low-logic level when SW is pressed */
#define SW_GPIO_PRESSED_VALUE 0U
#else
#define SW_GPIO               BOARD_USER_BUTTON_GPIO
#define SW_GPIO_PIN           BOARD_USER_BUTTON_GPIO_PIN
#define SW_NAME               BOARD_USER_BUTTON_NAME
/* GPIO port input high-logic level when SW is pressed */
#define SW_GPIO_PRESSED_VALUE 1U
#endif

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
