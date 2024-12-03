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
#define DEMO_EQDC EQDC0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
/* Initialize EQDC clock, return the clock frequency. */
uint32_t DEMO_InitEqdcClock(void);

/* Initializes the 1kHz TRIGGER signal, ready to start. */
void DEMO_InitTrigger(void);

/* Start the 1kHz TRIGGER signal. */
void DEMO_StartTrigger(void);

/* Initialize EQDC PHASEA and PHASEB signal connection. */
void DEMO_InitEqdcInputSignal(void);

void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
