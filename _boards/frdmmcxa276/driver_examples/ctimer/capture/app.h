/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_CTIMER   CTIMER0
#define DEMO_GPIO     GPIO1
#define DEMO_GPIO_PIN 9
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void DEMO_InitCtimerInput(void);
void DEMO_InitGpioPin(void);
void DEMO_PullGpioPin(int level);
/*${prototype:end}*/

#endif /* _APP_H_ */
