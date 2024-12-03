/*
 * Copyright 2021 NXP
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

#define BOARD_QTMR_BASEADDR TMRA
#define BOARD_QTMR_CHANNEL  kQTMR_Channel0

#define QTMR_PRIMARY_SOURCE       (kQTMR_PrimarySrcIPBusClockDivide2)
#define QTMR_CLOCK_SOURCE_DIVIDER (2U)
#define QTMR_SOURCE_CLOCK         (CLOCK_GetFreq(kCLOCK_BusClk) / QTMR_CLOCK_SOURCE_DIVIDER)
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
void ivINT_TMRA_0(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
