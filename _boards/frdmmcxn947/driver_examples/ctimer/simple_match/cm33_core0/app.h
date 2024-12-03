/*
 * Copyright 2022 NXP
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
#define CTIMER          CTIMER0         /* Timer 0 */
#define CTIMER_MAT_OUT  kCTIMER_Match_0 /* Match output 0 */
#define CTIMER_EMT_OUT  (1u << kCTIMER_Match_0)
#define CTIMER_CLK_FREQ CLOCK_GetCTimerClkFreq(0U)

#define LED_RED1_INIT   LED_RED_INIT
#define LED_RED1_ON     LED_RED_ON
#define LED_RED1_OFF    LED_RED_OFF
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
