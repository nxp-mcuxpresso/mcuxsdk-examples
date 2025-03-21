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
#define DEMO_ADC_BASE                 AON__LPADC0
#define DEMO_ADC_FIFO_INDEX           kADC_Fifo0
#define DEMO_ADC_DOZE_MODE_EN         true
#define DEMO_ADC_CAL_CONV_AVG         kADC_CalConvAvg128
#define DEMO_ADC_CAL_CONV_SAMP_TIME   kADC_CalConvSampTime_AdckCycle5
#define DEMO_ADC_VREF_SOURCE          kADC_RefVoltageAlt1
#define DEMO_ADC_FIFO0_WATERMARK      0U
#define DEMO_ADC_CHAN_INDEX           27U
#define DEMO_ADC_CONV_AVG             kADC_ConvAvg128
#define DEMO_ADC_CONV_SAMP_TIME       kADC_ConvSampleTime_AdcCycle8
#define DEMO_ADC_CMD_INDEX            kADC_Cmd1
#define DEMO_ADC_TRIG_INDEX           kADC_Trig0
#define DEMO_ADC_IRQ                  LPADC_AON_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
