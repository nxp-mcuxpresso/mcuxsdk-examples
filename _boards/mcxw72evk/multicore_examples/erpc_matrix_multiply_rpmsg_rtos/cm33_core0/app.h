/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_gpio.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define ERPC_TRANSPORT_RPMSG_LITE_LINK_ID (RL_PLATFORM_MCXW72X_LINK_ID)

#define BUTTON_INIT()                   \
    gpio_pin_config_t button_config = { \
        kGPIO_DigitalInput,             \
        0,                              \
    };                                  \
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &button_config)
#define IS_BUTTON_PRESSED() (0U == GPIO_PinRead(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN))
#define BUTTON_NAME         BOARD_SW3_NAME

/* Address where the image for core1 starts */
#define CORE1_BOOT_ADDRESS 0x48800000

extern char rpmsg_lite_base[];

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
