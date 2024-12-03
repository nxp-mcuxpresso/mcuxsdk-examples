/*
 * Copyright 2020 NXP
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
#define DEMO_QDC QDC
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
/* Initialize QDC clock, return the clock frequency. */
uint32_t DEMO_InitQdcClock(void);

/* Initializes the 1kHz TRIGGER signal, ready to start. */
void DEMO_InitTrigger(void);

/* Start the 1kHz TRIGGER signal. */
void DEMO_StartTrigger(void);

/* Initialize QDC PHASEA and PHASEB signal connection. */
void DEMO_InitQdcInputSignal(void);

void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
