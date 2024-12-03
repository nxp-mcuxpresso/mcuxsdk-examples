/*
 * Copyright 2023 NXP
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
/* Note that:
 * 1. The ADC instance number in following macros must be same:
 *    1) DEMO_ADC12_BASE;
 *    2) DEMO_ADC12_CLOCK_NAME;
 *    3) DEMO_ADC12_IRQn;
 *    4) DEMO_ADC12_IRQ_HANDLER_FUNC;
 * 2. The ADC12 has 4 conversion clock source:
 *    1) kADC12_ClockSourceAlt0(ADC_ALTCLK1);
 *    2) kADC12_ClockSourceAlt1(ADC_ALTCLK2);
 *    3) kADC12_ClockSourceAlt2(ADC_ALTCLK3);
 *    4) kADC12_ClockSourceAlt3(ADC_ALTCLK4);
 *    But only kADC12_ClockSourceAlt0(ADC_ALTCLK1) is workable on KE1xZ. kADC12_ClockSourceAlt0 also has 4
 *    inputs from PCC module, they are:
 *    i) System Oscillator async clock;
 *    ii) Slow IRC async clock;
 *    iii) Fast IRC async clock;
 *    iiii) System FLL async clock;
 */
#define DEMO_ADC12_BASE             ADC0
#define DEMO_ADC12_CLOCK_NAME       kCLOCK_Adc0
#define DEMO_ADC12_CLOCK_SOURCE     kADC12_ClockSourceAlt0
#define DEMO_ADC12_IRQn             ADC0_IRQn
#define DEMO_ADC12_IRQ_HANDLER_FUNC ADC0_IRQHandler

#define DEMO_ADC12_USER_CHANNEL  0U
#define DEMO_ADC12_CHANNEL_GROUP 0U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
