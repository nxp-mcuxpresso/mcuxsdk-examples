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
/* Choose ISR type. */
#define DSC_INTERRUPT_WEAK    0
#define DSC_INTERRUPT_ON      1
#define DSC_INTERRUPT_SAVEALL 0
#define DSC_INTERRUPT_FAST    0

#define DEMO_PIT_BASEADDR PIT0
#define PIT_LED_HANDLER   ivINT_PIT0_ROLLOVR

/* Set PIT Clock source divider to 32768, the value of two macros below should be aligned. */
#define PIT_CLOCK_SOURCE         kPIT_CountBusClk         /* 50M bus clock */
#define PIT_CLOCK_SOURCE_DIVIDER kPIT_PrescalerDivBy16384 /* divide source by 32768*/
/* Equation: interrupt rate = peripheral clock rate / ((2 ^ prescaler) * modulo value). */
#define PIT_PERIOD_COUNT 3052U /* 1s count of clock cycles, calculate by MSEC_TO_COUNT(1000, source_clock/divider)*/
#define PIT_IRQ_ID       PIT0_ROLLOVR_IRQn

#if defined(DSC_INTERRUPT_FAST) && (DSC_INTERRUPT_FAST == 1U)
#include "fsl_intc.h"
#define PIT_VECTOR_NUM PIT0_ROLLOVR_VECTORn
#define PIT_IRQ_PRIO   3 /* Priority level 2*/
#else
#define PIT_IRQ_PRIO 1   /* Priority level 0*/
#endif

/* Set XBARA. */
#define DEMO_XBARA_USER_CHANNEL_INPUT  kXBARA_InputPit0SyncOut
#define DEMO_XBARA_USER_CHANNEL_OUTPUT kXBARA_OutputXbOut8
#define DEMO_XBARA_BASEADDR            XBARA
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void LED_INIT(void);
void LED_TOGGLE(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
