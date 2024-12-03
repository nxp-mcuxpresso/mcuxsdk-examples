/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "board.h"
#include "pin_mux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define HAVE_RGB_LED

#define RGB_LED_RED   BOARD_LED_RED_PIN_MASK
#define RGB_LED_GREEN BOARD_LED_GREEN_PIN_MASK
#define RGB_LED_BLUE  BOARD_LED_BLUE_PIN_MASK

#define LED_On(mask)  GPIO_PortClear(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PORT, mask)
#define LED_Off(mask) GPIO_PortSet(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PORT, mask)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
