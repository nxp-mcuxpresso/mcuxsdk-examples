/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_sar_adc.h"
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
bool thrsholdIntFlag                   = false;
adc_channel_config_t adcChannelConfig[3U] = {
    {
        .channelIndex      = DEMO_ADC_CHANNEL0,
        .enableInt         = false,
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
void DEMO_ADC_IRQ_HANDLER_FUNC(void)
{
    if (kADC_NormalConvChainEndIntFlag == (ADC_GetConvIntStatus(DEMO_ADC_BASE) & kADC_NormalConvChainEndIntFlag))
    {
        ADC_ClearConvIntStatus(DEMO_ADC_BASE, kADC_NormalConvChainEndIntFlag);
        thrsholdIntFlag = true;
    }
}

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

    PRINTF("\r\n ADC interrupt example.");

    /* Configure the ADC. */
    ADC_GetDefaultConfig(&adcConfig);
    ADC_Init(DEMO_ADC_BASE, &adcConfig);

    /* Configure the ADC conversion chain. */
    chainConfig.convMode                      = kADC_NormalConvScanMode;
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

    /* Enable the interrupt. */
    EnableIRQ(DEMO_ADC_IRQn);

    /* Start the ADC conversion chain to execute the conversion. */
    ADC_StartConvChain(DEMO_ADC_BASE, kADC_NormalConvScanMode);

    PRINTF("\r\n Please press any key to get the conversion result.");

    while (1)
    {
        GETCHAR();

        while (!thrsholdIntFlag)
        {
        }

        thrsholdIntFlag = false;

        if (ADC_GetChannelConvResult(DEMO_ADC_BASE, &convResult, DEMO_ADC_CHANNEL0))
        {
            PRINTF("\r\n The channel %d ADC conversion data is %d.", DEMO_ADC_CHANNEL0, convResult.convData);
        }
        if (ADC_GetChannelConvResult(DEMO_ADC_BASE, &convResult, DEMO_ADC_CHANNEL1))
        {
            PRINTF("\r\n The channel %d ADC conversion data is %d.", DEMO_ADC_CHANNEL1, convResult.convData);
        }
        if (ADC_GetChannelConvResult(DEMO_ADC_BASE, &convResult, DEMO_ADC_CHANNEL2))
        {
            PRINTF("\r\n The channel %d ADC conversion data is %d.", DEMO_ADC_CHANNEL2, convResult.convData);
        }
    }
}
