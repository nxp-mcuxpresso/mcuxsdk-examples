/*
 * Copyright 2026 NXP
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
#define DEMO_PIT_BASEADDR PIT0
#define PIT_IRQ_ID        PIT0_IRQn
#define PIT_LED_HANDLER   PIT0_IRQHandler
#define PIT_CLOCK         kCLOCK_PIT1

#define PIT_CLOCK_SOURCE         kPIT_CountFroHfDivClk
#define PIT_CLOCK_SOURCE_DIVIDER kPIT_PrescalerDivBy8
/* Equation: modulo value = (peripheral clock rate / ((2 ^ prescaler) * interrupt rate)) - 1U */
#define PIT_PERIOD_COUNT (((48 * 1000 * 1000) >> 3) - 1)

#define PIT_IRQ_PRIO 1

#define LED_INIT()   LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE() LED_RED_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
