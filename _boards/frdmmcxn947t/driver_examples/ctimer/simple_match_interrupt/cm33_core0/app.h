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
#define CTIMER          CTIMER4         /* Timer 4 */
#define CTIMER_MAT0_OUT kCTIMER_Match_0 /* Match output 0 */
#define CTIMER_EMT0_OUT (1u << kCTIMER_Match_0)
#define CTIMER_MAT1_OUT kCTIMER_Match_1 /* Match output 1 */
#define CTIMER_EMT1_OUT (1u << kCTIMER_Match_1)
#define CTIMER_CLK_FREQ CLOCK_GetCTimerClkFreq(4U)

#define LED_RED1_INIT LED_RED_INIT
#define LED_RED1_ON   LED_RED_ON
#define LED_RED1_OFF  LED_RED_OFF
#define LED_RED2_INIT LED_BLUE_INIT
#define LED_RED2_ON   LED_BLUE_ON
#define LED_RED2_OFF  LED_BLUE_OFF
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void ctimer_match0_callback(uint32_t flags);
void ctimer_match1_callback(uint32_t flags);

/* Array of function pointers for callback for each channel */
ctimer_callback_t ctimer_callback_table[] = {
    ctimer_match0_callback, ctimer_match1_callback, NULL, NULL, NULL, NULL, NULL, NULL};
/*${prototype:end}*/

#endif /* _APP_H_ */
