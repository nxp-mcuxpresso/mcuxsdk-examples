/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "pin_mux.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*${macro:start}*/
#define BOARD_LED_GPIO     GPIOF
#define BOARD_LED_GPIO_PIN 1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
