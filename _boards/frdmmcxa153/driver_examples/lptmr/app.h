/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPTMR_BASE    LPTMR0
#define LPTMR_USEC_COUNT   1000000
#define DEMO_LPTMR_IRQn    LPTMR0_IRQn
#define LPTMR_LED_HANDLER  LPTMR0_IRQHandler
#define LPTMR_SOURCE_CLOCK (16000U)
#define LED_INIT()         LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE()       LED_RED_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
