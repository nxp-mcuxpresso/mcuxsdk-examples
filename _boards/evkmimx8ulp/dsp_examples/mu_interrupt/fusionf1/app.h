/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_MU              MU1_MUB
#define BOARD_LED_RGPIO     GPIOA
#define BOARD_LED_RGPIO_PIN 15U
#define APP_MU_IRQn         MU1_B_IRQn

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_MU_IRQHandler();
void LED_INIT();
void LED_TOGGLE();
/*${prototype:end}*/

#endif /* _APP_H_ */
