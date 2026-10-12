/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* TODO: SCTimerClock: 260MHz/(25+1) = 10MHz */
#define SCTIMER_CLK_PRESCALE    (25U)
#define SCTIMER_CLK_FREQ        CLOCK_GetCoreSysClkFreq()
#define SCTIMER_INPUT_PIN       kSCTIMER_Input_1

/* Interrupt number and interrupt handler for the SCT instance used */
#define SCTIMER_INPUT_CAPTURE_HANDLER SCT0_IRQHandler

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
