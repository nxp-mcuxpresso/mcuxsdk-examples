/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_OSTIMER_FREQ 16384
#define EXAMPLE_OSTIMER      OSTIMER0
#define EXAMPLE_OSTIMER_IRQn OS_EVENT_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/* Enter deep sleep mode. */
void EXAMPLE_EnterDeepSleep(void);
/* Enable OSTIMER IRQ under deep mode */
void EXAMPLE_EnableDeepSleepIRQ(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
