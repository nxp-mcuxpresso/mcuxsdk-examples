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
bool thrsholdIntFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_ADC_IRQ_HANDLER_FUNC(void)
{
    uint32_t intStatus = ADC_GetWdgThresholdIntStatus(DEMO_ADC_BASE);

    if (kADC_wdg0HighThresholdIntFlag == intStatus)
    {
        ADC_DisableWdgThresholdInt(DEMO_ADC_BASE, kADC_wdg0HighThresholdIntEnable);
        ADC_ClearWdgThresholdIntStatus(DEMO_ADC_BASE, intStatus);
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
    adc_channel_config_t adcChannelConfig;
    adc_wdg_config_t wdgConfig;
    adc_calibration_config_t calibrationConfig;
    adc_conv_result_t convResult;

    /* Initialize hardware. */
    BOARD_InitHardware();

    PRINTF("\r\n ADC watchdog example.");

    /* Configure the ADC. */
    ADC_GetDefaultConfig(&adcConfig);
    ADC_Init(DEMO_ADC_BASE, &adcConfig);

    /* 2. Configure the ADC conversion channels. */
    adcChannelConfig.channelIndex      = DEMO_ADC_CHANNEL;
    adcChannelConfig.enableInt         = false;
    adcChannelConfig.enablePresample   = false;
    adcChannelConfig.enableDmaTransfer = false;
    adcChannelConfig.enableWdg         = true;
    adcChannelConfig.wdgIndex          = DEMO_ADC_WDGID;

    /* Configure the ADC conversion chain. */
    chainConfig.convMode                      = kADC_NormalConvScanMode;
    chainConfig.enableGlobalChannelConvEndInt = false;
    chainConfig.enableChainConvEndInt         = false;
    chainConfig.channelCount                  = 1U;
    chainConfig.channelConfig                 = &adcChannelConfig;
    ADC_SetConvChainConfig(DEMO_ADC_BASE, &chainConfig);

    /* Configure the ADC watchdog. */
    wdgConfig.wdgIndex         = DEMO_ADC_WDGID;
    wdgConfig.wdgThresholdInt  = DEMO_ADC_WDG_INT_TYPE;
    wdgConfig.lowThresholdVal  = DEMO_ADC_LOW_THRESHOLD;
    wdgConfig.highThresholdVal = DEMO_ADC_HIGH_THRESHOLD;
    ADC_SetAnalogWdgConfig(DEMO_ADC_BASE, &wdgConfig);

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

    /* Enable the ADC conversion chain to execute the conversion. */
    ADC_StartConvChain(DEMO_ADC_BASE, kADC_NormalConvScanMode);

    PRINTF("\r\n Please press any key to get the conversion result.");

    while (1)
    {
        GETCHAR();

        if (ADC_GetChannelConvResult(DEMO_ADC_BASE, &convResult, DEMO_ADC_CHANNEL))
        {
            PRINTF("\r\n The channel %d ADC conversion value is %d.", DEMO_ADC_CHANNEL, convResult.convData);
        }

        if (thrsholdIntFlag)
        {
            PRINTF("\r\n Current ADC value is higher than the high threshold.");
            break;
        }
    }
    while (1)
    {
    }
}
