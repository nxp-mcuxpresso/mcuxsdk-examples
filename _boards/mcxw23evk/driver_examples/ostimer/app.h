/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_OSTIMER                OSTIMER
#define EXAMPLE_OSTIMER_FREQ           32768
#define EXAMPLE_EXCLUDE_FROM_DEEPSLEEP (kLOWPOWERCFG_FRO32K)
#define EXAMPLE_WAKEUP_IRQ_SOURCE      kWAKEUP_OS_EVENT
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
