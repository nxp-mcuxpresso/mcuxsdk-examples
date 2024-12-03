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
#define CTIMER          CTIMER3         /* Timer 3 */
#define CTIMER_MAT_OUT  kCTIMER_Match_3 /* Match output 3 */
#define CTIMER_CLK_FREQ CLOCK_GetCTimerClkFreq(3U)
#define CTIMER_MAT_PWM_PERIOD_CHANNEL kCTIMER_Match_1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
