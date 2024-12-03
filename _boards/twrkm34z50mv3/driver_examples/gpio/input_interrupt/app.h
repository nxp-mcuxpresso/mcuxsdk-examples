/*
 * Copyright 2021 NXP
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
#define BOARD_LED_GPIO     GPIOF
#define BOARD_LED_GPIO_PIN 1

#define BOARD_SW_GPIO        BOARD_SW1_GPIO
#define BOARD_SW_PORT        BOARD_SW1_PORT
#define BOARD_SW_GPIO_PIN    BOARD_SW1_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_SW1_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW1_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_SW1_NAME
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
