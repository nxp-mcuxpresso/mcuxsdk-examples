/*
 * Copyright 2024 NXP
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
#define CTIMER          CTIMER1         /* Timer 1 */
#define CTIMER_MAT_OUT  kCTIMER_Match_2 /* Match output 2 */
#define CTIMER_EMT_OUT  (1u << kCTIMER_Match_2)
#define CTIMER_CLK_FREQ CLOCK_GetCTimerClkFreq(1U)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
