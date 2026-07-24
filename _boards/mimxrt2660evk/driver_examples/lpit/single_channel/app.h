/*
 * Copyright 2025 NXP
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
#define DEMO_LPIT_BASE       HSP__LPIT_0
#define DEMO_LPIT_IRQn       HSP_LPIT0_IRQn
#define DEMO_LPIT_IRQHandler HSP_LPIT0_IRQHandler

/* Get source clock for LPIT driver */
#define LPIT_SOURCECLOCK 12000000

#define LED_INIT()   USER_LED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE() USER_LED_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
