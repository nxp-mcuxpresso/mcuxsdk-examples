/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_
#include "board.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_KBI                             KBI0
#define EXAMPLE_KBI_SIGNAL_INPUT_REF_GPIO       kGPIO_PORTB
#define EXAMPLE_KBI_SIGNAL_INPUT_REF_GPIO_INDEX 5
#define EXAMPLE_KBI_PINS                        (0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
