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

#define DEMO_USED_CHANNELS      3U
#define DEMO_ADC_CHANNEL0       48U
#define DEMO_ADC_CHANNEL1       55U
#define DEMO_ADC_CHANNEL2       5U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
