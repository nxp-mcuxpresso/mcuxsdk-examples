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
#define CTIMER          CTIMER1         /* Timer 1 */
#define CTIMER_MAT0_OUT kCTIMER_Match_2 /* Match output 2 */
#define CTIMER_EMT0_OUT (1u << kCTIMER_Match_2)
#define CTIMER_MAT1_OUT kCTIMER_Match_3 /* Match output 3 */
#define CTIMER_EMT1_OUT (1u << kCTIMER_Match_3)
#define CTIMER_CLK_FREQ CLOCK_GetCTimerClkFreq(1U)

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
    NULL, ctimer_match0_callback, NULL, ctimer_match1_callback, NULL, NULL, NULL, NULL};
/*${prototype:end}*/

#endif /* _APP_H_ */
