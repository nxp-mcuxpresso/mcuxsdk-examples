/*
 * Copyright 2017 NXP
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
#define DEMO_LPIT_BASE       LPIT1
#define DEMO_LPIT_IRQn       LPIT1_IRQn
#define DEMO_LPIT_IRQHandler LPIT1_IRQHandler
/* Get source clock for LPIT driver */
#define LPIT_SOURCECLOCK CLOCK_GetIpFreq(kCLOCK_Lpit1)
#define LED_INIT()       LED1_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE()     LED1_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void LED_Init(void);
void LED_Toggle(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
