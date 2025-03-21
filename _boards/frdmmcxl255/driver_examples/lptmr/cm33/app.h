/*
 * Copyright 2023-2025 NXP
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
#define DEMO_LPTMR_BASE    AON__LPTMR0
#define LPTMR_USEC_COUNT   1000000
#define DEMO_LPTMR_IRQn    LPTMR_AON_IRQn
#define LPTMR_LED_HANDLER  LPTMR_AON_IRQHandler
#define LPTMR_SOURCE_CLOCK (CLOCK_GetFreq(kCLOKC_FroAON)/4U)
#define LED_INIT()         LED_BLUE_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE()       LED_BLUE_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
