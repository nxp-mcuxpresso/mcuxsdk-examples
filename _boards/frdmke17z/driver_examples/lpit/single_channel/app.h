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
#define DEMO_LPIT_BASE       LPIT0
#define DEMO_LPIT_IRQn       LPIT0_IRQn
#define DEMO_LPIT_IRQHandler LPIT0_IRQHandler
/* Get source clock for LPIT driver */
#define LPIT_SOURCECLOCK CLOCK_GetFreq(kCLOCK_ScgSircAsyncDiv2Clk)
#define LED_INIT()       LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE()     LED_RED_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
