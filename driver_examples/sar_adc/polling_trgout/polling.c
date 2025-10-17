/*
 * Copyright 2023-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_sar_adc.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_xbar.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
adc_channel_config_t adcChannelConfig[3U] = {
    {
        .channelIndex      = DEMO_ADC_CHANNEL0,
        .enableInt         = true,
        .enablePresample   = false,
        .enableDmaTransfer = false,
        .enableWdg         = false,
        .wdgIndex          = 0U,
    },
    {
        .channelIndex      = DEMO_ADC_CHANNEL1,
        .enableInt         = false,
        .enablePresample   = false,
        .enableDmaTransfer = false,
        .enableWdg         = false,
        .wdgIndex          = 0U,
    },
    {
        .channelIndex      = DEMO_ADC_CHANNEL2,
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

/*!
 * @brief Main function
 */
int main(void)
{
    adc_config_t adcConfig;
    adc_chain_config_t chainConfig;
    adc_calibration_config_t calibrationConfig;
    adc_conv_result_t convResult;

    /* Initialize hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    XBAR_Init(kXBAR_DSC1);
    BLK_CTRL_WAKEUPMIX->XBAR_DIR_CTRL1 |= BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL1_IOMUXC_XBAR_DIR_SEL_8(1);
    XBAR_SetSignalsConnection(kXBAR1_InputAdc1IpiIntEoc, kXBAR1_OutputIomuxXbarOut08);
    PRINTF("\r\nIPSYNC trigger signal connected to output \r\n");

    PRINTF("\r\n ADC polling example.");

    /* Configure the ADC. */
    ADC_GetDefaultConfig(&adcConfig);
    ADC_Init(DEMO_ADC_BASE, &adcConfig);

    /* Configure the ADC conversion chain. */
    chainConfig.convMode                      = kADC_NormalConvOneShotMode;
    chainConfig.enableGlobalChannelConvEndInt = false;
    chainConfig.enableChainConvEndInt         = true;
    chainConfig.channelCount                  = DEMO_USED_CHANNELS;
    chainConfig.channelConfig                 = &(adcChannelConfig[0U]);
    ADC_SetConvChainConfig(DEMO_ADC_BASE, &chainConfig);

    /* Do calibration to reduce or eliminate the various error contribution effects. */
    calibrationConfig.enableAverage        = true;
    calibrationConfig.sampleTime           = kADC_SampleTime22;
    calibrationConfig.averageSampleNumbers = kADC_AverageSampleNumbers512;

    ADC_SetPowerDownMode(DEMO_ADC_BASE, false);

    if (!(ADC_DoCalibration(DEMO_ADC_BASE, &calibrationConfig)))
    {
        PRINTF(
            "\r\n Calibration failed, please call function 'ADC_GetCalibrationLastFailedTestResult' \
        and 'ADC_GetCalibrationStepsStatus' to get failed information.");
    }

    while (1)
    {
        PRINTF("\r\n Please press any key to get the conversion result.");

        GETCHAR();

        /* Start the ADC conversion chain to execute the conversion. */
        ADC_StartConvChain(DEMO_ADC_BASE, kADC_NormalConvOneShotMode);

        while (!ADC_GetChannelConvResult(DEMO_ADC_BASE, &convResult, DEMO_ADC_CHANNEL0))
        {
        }
        PRINTF("\r\n The channel %d ADC conversion data is %d.", DEMO_ADC_CHANNEL0, convResult.convData);

        while (!ADC_GetChannelConvResult(DEMO_ADC_BASE, &convResult, DEMO_ADC_CHANNEL1))
        {
        }
        PRINTF("\r\n The channel %d ADC conversion data is %d.", DEMO_ADC_CHANNEL1, convResult.convData);

        while (!ADC_GetChannelConvResult(DEMO_ADC_BASE, &convResult, DEMO_ADC_CHANNEL2))
        {
        }
        PRINTF("\r\n The channel %d ADC conversion data is %d.", DEMO_ADC_CHANNEL2, convResult.convData);

	ADC_ClearConvIntStatus(DEMO_ADC_BASE, kADC_NormalConvChainEndIntFlag);
    }
}
