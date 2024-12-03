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
#define DEMO_LPTMR_BASE    LPTMR0
#define LPTMR_USEC_COUNT   1000000
#define DEMO_LPTMR_IRQn    LPTMR0_IRQn
#define LPTMR_LED_HANDLER  LPTMR0_IRQHandler
#define LPTMR_SOURCE_CLOCK (CLOCK_GetFreq(kCLOCK_ScgSircClk))
#define LED_INIT()         LED3_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE()       LED3_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
