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
#define BOARD_LED_GPIO       GPIOA
#define BOARD_LED_GPIO_PIN   8U
#define BOARD_SW_GPIO        BOARD_VOLP_GPIO
#define BOARD_SW_PORT        BOARD_VOLP_PORT
#define BOARD_SW_GPIO_PIN    BOARD_VOLP_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_VOLP_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_VOLP_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_VOLP_NAME
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
