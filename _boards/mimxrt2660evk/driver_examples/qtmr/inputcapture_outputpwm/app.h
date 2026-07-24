/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_QTMR_BASEADDR              HSP__QTMR_0
#define BOARD_QTMR_INPUT_CAPTURE_CHANNEL kQTMR_Channel_0
#define BOARD_QTMR_PWM_CHANNEL           kQTMR_Channel_1
#define QTMR_PWM_OUTPUT_FREQUENCY        50000
#define QTMR_DUTYCYCLE_PERCENT           50
#define QTMR_CounterInputPin             kQTMR_Counter0InputPin

#define QTMR_IRQ_ID      HSP_QTMR0_IRQn
#define QTMR_IRQ_HANDLER HSP_QTMR0_IRQHandler

#define QTMR_PRIMARY_SOURCE       (kQTMR_ClockDivide_128)
#define QTMR_CLOCK_SOURCE_DIVIDER (128U)

#define QTMR_SOURCE_CLOCK (CLOCK_GetRootClockFreq(kCLOCK_Root_CGU_MAIN_ROOTCLK) / QTMR_CLOCK_SOURCE_DIVIDER)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
