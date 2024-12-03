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
#define DEMO_COP_BASE COP
#define DEMO_COP_IRQ_HANDLER ivINT_COP_INT

#define COP_SOURCE_CLOCK kCOP_RoscClockSource /* 8M FROSC clock */
#define COP_SOURCE_DIVIDER kCOP_ClockPrescalerDivide1024 /* Divide source clock by 1024 */
#define COP_TIMEOUT_COUNT 31250U  /* 4s count of clock cycles, calculate by MSEC_TO_COUNT(4000, source_clock/divider)*/
#define COP_INTERRUPT_COUNT  15625U /* 2s count of clock cycles, calculate by MSEC_TO_COUNT(2000, source_clock/divider)*/
#define COP_IRQ_ID COP_INT_IRQn
#define COP_IRQ_PRIO 1
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
