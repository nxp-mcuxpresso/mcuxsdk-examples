/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_bctu.h"
#include "fsl_sar_adc.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void DEMO_AdcConfig(void)
{
    adc_config_t adcConfig;
    adc_calibration_config_t calibrationConfig;

    ADC_GetDefaultConfig(&adcConfig);
    adcConfig.clockFrequency    = DEMO_ADC_CLOCK_FREQ;
    adcConfig.bctuMode          = DEMO_ADC_BCTU_MODE;
    adcConfig.convAvg           = DEMO_ADC_CONV_AVERAGE;
    ADC_Init(DEMO_ADC_BASE, &adcConfig);

    calibrationConfig.enableAverage        = DEMO_ADC_CAL_AVERAGE_ENABLE;
    calibrationConfig.sampleTime           = DEMO_ADC_CAL_SAMPLE_TIME;
    calibrationConfig.averageSampleNumbers = DEMO_ADC_CAL_AVERAGE_SAMPLE_NUM;

    if (!(ADC_DoCalibration(DEMO_ADC_BASE, &calibrationConfig)))
    {
        (void)PRINTF("Calibration failed\r\n");
    }
}

static void DEMO_BctuConfig(void)
{
    bctu_config_t config;
    bctu_trig_config_t trigConfig;

    BCTU_GetDefaultConfig(&config);
    config.writeProtect = kBCTU_ProtectDis_Permanent;
    BCTU_Init(DEMO_BCTU_BASE, &config);

    trigConfig.trigIndex = DEMO_BCTU_TRIGGER_INDEX;
    trigConfig.chanAddr = DEMO_BCTU_TRIG_ADC_ADDR;
    trigConfig.dataDest = DEMO_BCTU_DATA_DEST;
    trigConfig.enableLoop = DEMO_BCTU_LOOP_ENABLE;
    trigConfig.targetAdc = (uint32_t)DEMO_BCTU_TRIG_ADC_INSTANCE;
    trigConfig.trigRes = DEMO_BCTU_TRIG_RESOLUTION;
    BCTU_SetTrigConfig(DEMO_BCTU_BASE, &trigConfig);
}

/*!
 * @brief Main function
 */
int main(void)
{
    bctu_adc_conv_result_t result;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    DEMO_AdcConfig();
    DEMO_BctuConfig();

    BCTU_EnableModule(DEMO_BCTU_BASE, true);
    BCTU_EnableGlobalTrig(DEMO_BCTU_BASE, true);

    (void)PRINTF("BCTU SoftWare Trigger Example\r\n");
    (void)PRINTF("Please press any key to get the ADC value\r\n");

    while (true)
    {
        (void)GETCHAR();

        BCTU_EnableSoftwareTrig(DEMO_BCTU_BASE, DEMO_BCTU_SW_TRIG_GROUP, DEMO_BCTU_SW_TRIG_MASK);

        while((BCTU_GetStatusFlags(DEMO_BCTU_BASE) & DEMO_BCTU_INT_MASK) != DEMO_BCTU_INT_MASK)
        {
        }

        BCTU_ClearStatusFlags(DEMO_BCTU_BASE, DEMO_BCTU_INT_MASK);
        BCTU_GetConvResult(DEMO_BCTU_BASE, DEMO_BCTU_TRIG_ADC_INSTANCE, &result);

        (void)PRINTF("ADC channel %d value: %d\r\n", result.chanNum, result.data);
    }
}
