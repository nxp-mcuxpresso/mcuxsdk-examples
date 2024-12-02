/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define EXAMPLE_EWM EWM
#define SW_GPIO     BOARD_VOLP_GPIO
#define SW_GPIO_PIN BOARD_VOLP_GPIO_PIN
#define SW_NAME     BOARD_VOLP_NAME
/* GPIO port input high-logic level when SW is pressed */
#define SW_GPIO_PRESSED_VALUE 1U

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
