/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/**
 * @brief Start address of non-secure application.
 */
#define mainNONSECURE_APP_START_ADDRESS DEMO_CODE_START_NS

/**
 * @brief LED port and pins.
 */
#define GREEN_LED_PORT BOARD_LED_GREEN_GPIO_PORT
#define BLUE_LED_PORT  BOARD_LED_BLUE_GPIO_PORT
#define GREEN_LED_PIN  BOARD_LED_GREEN_GPIO_PIN
#define BLUE_LED_PIN   BOARD_LED_BLUE_GPIO_PIN
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
