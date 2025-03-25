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
#define DEMO_ADC_BASE             ADC_0
#define DEMO_ADC_IRQn             ADC0_IRQn
#define DEMO_ADC_IRQ_HANDLER_FUNC ADC0_IRQHandler
#define DEMO_ADC_CONV_CLOCK_FREQ  kADC_ModuleClockFreqDivide2

#define DEMO_ADC_CHANNEL      5U
#define DEMO_ADC_WDGID        0U
#define DEMO_ADC_WDG_INT_TYPE kADC_HighThresholdIntEnable

/* ADC conversion result range: [0, 32767] */
#define DEMO_ADC_LOW_THRESHOLD  0U
#define DEMO_ADC_HIGH_THRESHOLD 24000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
