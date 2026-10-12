/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* On FPGA the OS timer is clocked from the main clock, not from a 32 kHz crystal. */
#define EXAMPLE_OSTIMER_FREQ           DEFAULT_SYSTEM_CLOCK
#define EXAMPLE_EXCLUDE_FROM_DEEPSLEEP (kPDRUNCFG_PD_XTAL32K)
#define EXAMPLE_WAKEUP_IRQ_SOURCE      WAKEUP_OS_EVENT_TIMER
#define EXAMPLE_OSTIMER                CTRLSS__OSTIMER_0
#define EXAMPLE_OSTIMER_IRQn           OS_EVENT_IRQn
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
