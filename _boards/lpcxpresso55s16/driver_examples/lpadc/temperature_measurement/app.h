/*
 * Copyright 2019-2020, 2024 NXP
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
#define DEMO_LPADC_BASE                  ADC0
#define DEMO_LPADC_IRQn                  ADC0_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC      ADC0_IRQHandler
#define DEMO_LPADC_TEMP_SENS_CHANNEL     26U
#define DEMO_LPADC_USER_CMDID            1U /* CMD1 */
#define DEMO_LPADC_SAMPLE_CHANNEL_MODE   kLPADC_SampleChannelDiffBothSide
#define DEMO_LPADC_VREF_SOURCE           kLPADC_ReferenceVoltageAlt3
#define DEMO_LPADC_DO_OFFSET_CALIBRATION false
#define DEMO_LPADC_OFFSET_VALUE_A        0x10U
#define DEMO_LPADC_OFFSET_VALUE_B        0x10U
#define DEMO_LPADC_USE_HIGH_RESOLUTION   true

#define DEMO_LPADC_TEMP_PARAMETER_A     FSL_FEATURE_LPADC_TEMP_PARAMETER_A
#define DEMO_LPADC_TEMP_PARAMETER_B     FSL_FEATURE_LPADC_TEMP_PARAMETER_B
#define DEMO_LPADC_TEMP_PARAMETER_ALPHA FSL_FEATURE_LPADC_TEMP_PARAMETER_ALPHA
#define DEMO_LPADC_HARDWARE_AVERAGE     kLPADC_HardwareAverageCount128
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
float DEMO_MeasureTemperature(ADC_Type *base, uint32_t commandId, uint32_t index);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
