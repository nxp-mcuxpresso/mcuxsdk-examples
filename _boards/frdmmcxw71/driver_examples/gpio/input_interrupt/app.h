/*
 * Copyright 2024 NXP
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
#define BOARD_SW_GPIO        BOARD_SW2_GPIO
#define BOARD_SW_PORT        BOARD_SW2_PORT
#define BOARD_SW_GPIO_PIN    BOARD_SW2_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_SW2_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW2_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_SW2_NAME

#define BOARD_LED_GPIO     BOARD_LED1_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED1_GPIO_PIN
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
