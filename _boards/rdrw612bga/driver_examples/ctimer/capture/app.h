/*
 * Copyright 2023 NXP
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
#define DEMO_CTIMER CTIMER0
/* GPIO52: Port 1 Pin 20*/
#define DEMO_GPIO_PORT 1
#define DEMO_GPIO_PIN  20
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
