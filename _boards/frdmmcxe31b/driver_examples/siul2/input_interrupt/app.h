/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_SIUL2_BASE     SIUL2
#define BOARD_LED_GPIO       BOARD_LED_RED_GPIO 
#define BOARD_LED_GPIO_PIN   BOARD_LED_RED_GPIO_PIN
#define BOARD_SW_EIRQ        13U /* EIRQ13*/
#define BOARD_SW_IRQ         SIUL2_1_IRQn
#define BOARD_SW_IRQ_HANDLER SIUL2_1_IRQHandler
#define BOARD_SW_NAME        "SW3"
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
