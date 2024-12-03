/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
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
#define ERPC_TRANSPORT_RPMSG_LITE_LINK_ID (RL_PLATFORM_K32L3A60_M4_M0_LINK_ID)

#define BUTTON_INIT()                   \
    gpio_pin_config_t button_config = { \
        kGPIO_DigitalInput,             \
        0,                              \
    };                                  \
    GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, &button_config)
#define IS_BUTTON_PRESSED() (0U == GPIO_PinRead(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN))
#define BUTTON_NAME         BOARD_SW2_NAME

/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS 0x01000000

extern char rpmsg_lite_base[];

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_CORE0_H_ */
