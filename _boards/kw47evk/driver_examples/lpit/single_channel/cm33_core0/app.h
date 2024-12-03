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
#define DEMO_LPIT_BASE       LPIT0
#define DEMO_LPIT_IRQn       LPIT0_IRQn
#define DEMO_LPIT_IRQHandler LPIT0_IRQHandler
/* Get source clock for LPIT driver */
#define LPIT_SOURCECLOCK (CLOCK_GetFreq(kCLOCK_ScgSircClk))
#define LED_INIT()       LED3_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE()     LED3_TOGGLE()
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
