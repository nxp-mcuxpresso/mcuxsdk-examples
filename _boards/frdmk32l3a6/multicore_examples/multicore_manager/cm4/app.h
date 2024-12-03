/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_CORE0_H_
#define _APP_CORE0_H_

/*${header:start}*/
#include "fsl_gpio.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS 0x01000000

#define BUTTON_1_INIT()                                    \
    gpio_pin_config_t sw_config = {kGPIO_DigitalInput, 0}; \
    GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, &sw_config)
#define IS_BUTTON_1_PRESSED() (0U == GPIO_PinRead(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN))
#define BUTTON_1_NAME         "SW2"

#define BUTTON_2_INIT()       GPIO_PinInit(GPIOE, 12U, &sw_config)
#define IS_BUTTON_2_PRESSED() (0U == GPIO_PinRead(GPIOE, 12U))
#define BUTTON_2_NAME         "SW5"
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_CORE0_H_ */
