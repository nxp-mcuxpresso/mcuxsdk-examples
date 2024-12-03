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
#define QTMR_IRQ_ID   TMRA_2_IRQn
#define QTMR_IRQ_PRIO 1

#define BOARD_QTMR_BASEADDR TMRA
#define BOARD_QTMR_CHANNEL  kQTMR_Channel2

/* QTMR Clock source divider for Ip bus clock source, the value of two macros below should be aligned. */
#define QTMR_PRIMARY_SOURCE       (kQTMR_PrimarySrcIPBusClockDivide2)
#define QTMR_CLOCK_SOURCE_DIVIDER (2U)
#define QTMR_SOURCE_CLOCK         (CLOCK_GetFreq(kCLOCK_BusClk) / QTMR_CLOCK_SOURCE_DIVIDER)

/* The frequency of the PWM signal QTMR generated. */
#define QTMR_PWM_OUTPUT_FREQUENCY (50000U)
/* The dutycycle of the PTM signal QTMR generated. */
#define QTMR_DUTYCYCLE_PERCENT (50U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void QTMR_IRQ_HANDLER(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
