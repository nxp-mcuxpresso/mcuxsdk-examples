/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_OPAMP_INSTANCE0      OPAMP0
#define DEMO_OPAMP_INSTANCE1      OPAMP1
#define DEMO_OPAMP_COMP_CAP       kOPAMP_FitGain8x
#define DEMO_OPAMP_BIAS_CURRENT   kOPAMP_NoChange
#define DEMO_OPAMP_RESISTOR_2     (10000U)
#define DEMO_OPAMP_RESISTOR_1     (1000U)
#define DEMO_OPAMP_NEGATIVE_VALUE (0U) /* Connect to GND */

#define DEMO_LPADC_INSTANCE0                 ADC0
#define DEMO_LPADC_INSTANCE1                 ADC1
#define DEMO_LPADC_ENABLE_ANALOG_PRELIMINARY true
#define DEMO_LPADC_VREF_SOURCE               kLPADC_ReferenceVoltageAlt1
#define DEMO_LPADC_CONV_AVGS_MODE            kLPADC_ConversionAverage1024
#define DEMO_LPADC_DO_OFFSET_CALIBRATION     true
#define DEMO_LPADC_USE_HIGH_RESOLUTION       true

#define DEMO_LPADC_SAMPLE_CHANNEL_MODE        kLPADC_SampleChannelSingleEndSideA
#define DEMO_LPADC_CONVERSION_RESOLUTION_MODE kLPADC_ConversionResolutionHigh
#define DEMO_LPADC_HARDWARE_AVERAGE_MODE      kLPADC_HardwareAverageCount1024
#define DEMO_LPADC_VREF_VALUE                 (3283U)
#define DEMO_LPADC_ENABLE_HARDWARE_TRIGGER    false
#define DEMO_LPADC_OPAMP0_OUTPUT_CHANNEL      (2U) /* ADC0_A2 */
#define DEMO_LPADC_OPAMP1_OUTPUT_CHANNEL      (2U) /* ADC1_A2 */
#define DEMO_LPADC_OPAMP0_OUTPUT_CMDID        (1U)
#define DEMO_LPADC_OPAMP1_OUTPUT_CMDID        (1U)
#define DEMO_LPADC_OPAMP0_OUTPUT_TRIGGERID    (0U)
#define DEMO_LPADC_OPAMP1_OUTPUT_TRIGGERID    (0U)

#define DEMO_LPADC_OPAMP1_POSITIVE_INPUT_CMDID     (2U)
#define DEMO_LPADC_OPAMP1_POSITIVE_INPUT_CHANNEL   (1U) /* ADC0_A1 */
#define DEMO_LPADC_OPAMP1_POSITIVE_INPUT_TRIGGERID (0U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
