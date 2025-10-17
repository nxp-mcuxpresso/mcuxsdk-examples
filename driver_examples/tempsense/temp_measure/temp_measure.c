/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_sar_adc.h"
#include "fsl_tempsense.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
adc_channel_config_t adcChannelConfig[1U] = {
    {
        .channelIndex      = DEMO_ADC_CHANNEL0,
        .enableInt         = false,
        .enablePresample   = false,
        .enableDmaTransfer = false,
        .enableWdg         = false,
        .wdgIndex          = 0U,
    },
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void DEMO_AdcConfig(void)
{
    adc_config_t adcConfig;
    adc_chain_config_t chainConfig;
    adc_calibration_config_t calibrationConfig;

    /* Configure the ADC. */
    ADC_GetDefaultConfig(&adcConfig);
    adcConfig.clockFrequency    = DEMO_ADC_CLOCK_FREQ;
#if defined(FSL_FEATURE_ADC_HAS_MCR_AVGS) && (FSL_FEATURE_ADC_HAS_MCR_AVGS==1U)
    adcConfig.convAvg   = kADC_ConvAvg32;
#endif /* FSL_FEATURE_ADC_HAS_MCR_AVGS */
    ADC_Init(DEMO_ADC_BASE, &adcConfig);

    /* Configure the ADC conversion chain. */
    chainConfig.convMode                      = kADC_NormalConvOneShotMode;
    chainConfig.enableGlobalChannelConvEndInt = false;
    chainConfig.enableChainConvEndInt         = false;
    chainConfig.channelCount                  = DEMO_USED_CHANNELS;
    chainConfig.channelConfig                 = &(adcChannelConfig[0U]);
    ADC_SetConvChainConfig(DEMO_ADC_BASE, &chainConfig);

    /* Do calibration to reduce or eliminate the various error contribution effects. */
    calibrationConfig.enableAverage        = true;
    calibrationConfig.sampleTime           = kADC_SampleTime22;
#if (defined(FSL_FEATURE_ADC_HAS_CALBISTREG) && (FSL_FEATURE_ADC_HAS_CALBISTREG==1U))
    calibrationConfig.averageSampleNumbers = kADC_AverageSampleNumbers32;
#else
    calibrationConfig.averageSampleNumbers = kADC_AverageSampleNumbers512;
#endif /* FSL_FEATURE_ADC_HAS_CALBISTREG */

    if (!(ADC_DoCalibration(DEMO_ADC_BASE, &calibrationConfig)))
    {
        PRINTF(
            "\r\n Calibration failed, please call function 'ADC_GetCalibrationLastFailedTestResult' \
        and 'ADC_GetCalibrationStepsStatus' to get failed information.");
    }
}

static void DEMO_TempSenseConfig(void)
{
    tempsense_config_t tempsenseConfig;

    TEMPSENSE_GetDefaultConfig(&tempsenseConfig);
    tempsenseConfig.exposeGround = false;
    TEMPSENSE_Init(DEMO_TEMPSENSE_BASE, &tempsenseConfig);

    TEMPSENSE_EnableTempsense(DEMO_TEMPSENSE_BASE, true);
}

/*!
 * @brief Main function
 */
int main(void)
{
    adc_conv_result_t convResult;
   
    /* Initialize hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    DEMO_AdcConfig();
    DEMO_TempSenseConfig();

    PRINTF("\r\n Tempsense example.");

    while (1)
    {
        PRINTF("\r\n Please press any key to get the current temperature.");

        GETCHAR();

        ADC_StartConvChain(DEMO_ADC_BASE, kADC_NormalConvOneShotMode);

        while (!ADC_GetChannelConvResult(DEMO_ADC_BASE, &convResult, DEMO_ADC_CHANNEL0))
        {
        }
        PRINTF("\r\n Current temperature is %.3f.", \
          (double)TEMPSENSE_GetCurrentTemperature(DEMO_TEMPSENSE_BASE, convResult.convData, DEMO_ADC_VREF, DEMO_ADC_RESOLUTION));
    }
}
