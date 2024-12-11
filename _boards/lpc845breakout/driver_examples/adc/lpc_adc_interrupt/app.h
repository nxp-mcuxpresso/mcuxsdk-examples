/*
 * Copyright 2019, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_ADC_BASE                  ADC0
#define DEMO_ADC_SAMPLE_CHANNEL_NUMBER 0U
#define DEMO_ADC_IRQ_ID                ADC0_SEQA_IRQn
#define DEMO_ADC_IRQ_HANDLER_FUNC      ADC0_SEQA_IRQHandler
#define DEMO_ADC_CLOCK_SOURCE          kCLOCK_Fro
#define DEMO_ADC_CLOCK_DIVIDER         1U

#define DEMO_ADC_CLOCK_MODE kADC_ClockAsynchronousMode
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
