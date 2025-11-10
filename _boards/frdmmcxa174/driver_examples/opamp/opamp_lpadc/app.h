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
#define DEMO_OPAMP_INSTANCE0            OPAMP0
#define DEMO_OPAMP_COMP_CAP             kOPAMP_FitGain8x
#define DEMO_OPAMP_BIAS_CURRENT         kOPAMP_NoChange
#define DEMO_OPAMP_RESISTOR_3           (20000.0F)
#define DEMO_OPAMP_RESISTOR_2           (10000.0F)
#define DEMO_OPAMP_RESISTOR_1           (980.0F)
#define DEMO_OPAMP_REFERENCE            (3286.0F)

#define DEMO_LPADC_INSTANCE0                    ADC0
#define DEMO_LPADC_ENABLE_HARDWARE_TRIGGER      false
#define DEMO_LPADC_ENABLE_ANALOG_PRELIMINARY    true
#define DEMO_LPADC_DO_OFFSET_CALIBRATION        true
#define DEMO_LPADC_USE_HIGH_RESOLUTION          true
#define DEMO_LPADC_VREF_SOURCE                  kLPADC_ReferenceVoltageAlt1
#define DEMO_LPADC_CONV_AVGS_MODE               kLPADC_ConversionAverage1024
#define DEMO_LPADC_POWER_MODE                   kLPADC_PowerLevelAlt2
#define DEMO_LPADC_SAMPLE_TIME_MODE             kLPADC_SampleTimeADCK131
#define DEMO_LPADC_SAMPLE_CHANNEL_MODE          kLPADC_SampleChannelSingleEndSideA
#define DEMO_LPADC_CONVERSION_RESOLUTION_MODE   kLPADC_ConversionResolutionHigh
#define DEMO_LPADC_HARDWARE_AVERAGE_MODE        kLPADC_HardwareAverageCount1024
#define DEMO_LPADC_VREF_VALUE                   (3286.0F)

#define DEMO_LPADC_OPAMP_OUTPUT_CHANNEL                (2U)
#define DEMO_LPADC_OPAMP_NEGATIVE_INPUT_CHANNEL        (1U)
#define DEMO_LPADC_OPAMP_OUTPUT_CMDID                  (1U)
#define DEMO_LPADC_OPAMP_NEGATIVE_INPUT_CMDID          (2U)
#define DEMO_LPADC_OPAMP_OUTPUT_TRIGGERID              (0U)
#define DEMO_LPADC_OPAMP_NEGATIVE_INPUT_TRIGGERID      (0U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
