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
#define QTMR_IRQ_ID      TMRA_1_IRQn
#define QTMR_IRQ_PRIO    1
#define QTMR_IRQ_HANDLER ivINT_TMRA_1

#define BOARD_QTMR_BASEADDR       TMRA
#define BOARD_FIRST_QTMR_CHANNEL  kQTMR_Channel0
#define BOARD_SECOND_QTMR_CHANNEL kQTMR_Channel1
#define QTMR_ClockCounterOutput   kQTMR_PrimarySrcChannel0Output
/* QTMR Clock source divider for Ip bus clock source, the value of two macros below should be aligned. */
#define QTMR_PRIMARY_SOURCE       (kQTMR_PrimarySrcIPBusClockDivide128)
#define QTMR_CLOCK_SOURCE_DIVIDER (128U)
#define QTMR_SOURCE_CLOCK         (CLOCK_GetFreq(kCLOCK_BusClk) / QTMR_CLOCK_SOURCE_DIVIDER)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
