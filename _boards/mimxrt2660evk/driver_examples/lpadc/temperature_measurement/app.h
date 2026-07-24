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
#define DEMO_LPADC_BASE                    HSP__ADC_0
#define DEMO_LPADC_IRQn                    HSP_ADC0_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC        HSP_ADC0_IRQHandler
#define DEMO_LPADC_TEMP_SENS_CHANNEL       9U /* CH9 is connected to the internal temperature sensor. */
#define DEMO_LPADC_USER_CMDID              1U  /* CMD1 */
#define DEMO_LPADC_SAMPLE_CHANNEL_MODE     kLPADC_SampleChannelDiffBothSide
#define DEMO_LPADC_VREF_SOURCE             kLPADC_ReferenceVoltageAlt1
#define DEMO_LPADC_USE_HIGH_RESOLUTION     true
#define DEMO_LPADC_OFFSET_CALIBRATION_MODE kLPADC_OffsetCalibration16bitMode
#define DEMO_LPADC_DO_OFFSET_CALIBRATION   true
#define DEMO_LPADC_HARDWARE_AVERAGE        kLPADC_HardwareAverageCount128

/*
 * !IMPORTANT!
 * Parameter values follow RT700/RT1180 (same sensor IP); to be
 * re-confirmed against RT2660 datasheet ADC Electricals
 */
#define DEMO_LPADC_TEMP_PARAMETER_A        (789.2)
#define DEMO_LPADC_TEMP_PARAMETER_B        (319.2)
#define DEMO_LPADC_TEMP_PARAMETER_ALPHA    (11.2)
#define FSL_FEATURE_LPADC_TEMP_SENS_BUFFER_SIZE (2)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
float DEMO_MeasureTemperature(ADC_Type *base, uint32_t commandId, uint32_t index);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
