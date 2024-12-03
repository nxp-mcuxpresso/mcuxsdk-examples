/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
#define BOARD_SW_GPIO        BOARD_SW8_GPIO
#define BOARD_SW_GPIO_PIN    BOARD_SW8_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_SW8_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW8_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_SW8_NAME

#define BOARD_SW_INT_OUTPUT kRGPIO_InterruptOutput2
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
