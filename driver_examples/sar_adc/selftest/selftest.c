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
static void DEMO_ADC_SelfTestHandler(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
bool thrsholdIntFlag                   = false;
volatile uint32_t intStatus            = 0x00U;
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
    intStatus = ADC_GetSelfTestIntStatus(DEMO_ADC_BASE);

    if (0U != (intStatus & kADC_WdgSequenceErrIntEnable))
    {
        thrsholdIntFlag = true;
        ADC_ClearSelfTestIntStatus(DEMO_ADC_BASE, intStatus);
        DisableIRQ(DEMO_ADC_IRQn);
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
    adc_self_test_config_t selftestConfig;
    adc_conv_result_t convResult;
    uint8_t convChainRoundCount = 0U;

    /* Initialize hardware. */
    BOARD_InitHardware();

    PRINTF("\r\n ADC selftest example.");

    /* Configure the ADC. */
    ADC_GetDefaultConfig(&adcConfig);
    ADC_Init(DEMO_ADC_BASE, &adcConfig);

    /* Configure the ADC conversion chain. */
    chainConfig.convMode                      = kADC_NormalConvScanMode;
    chainConfig.enableGlobalChannelConvEndInt = false;
    chainConfig.enableChainConvEndInt         = false;
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

    /* Configure the self-test feature. */
    selftestConfig.algType                 = kADC_SelfTestForAlgSAndC;
    selftestConfig.algSteps                = 0x00U;
    selftestConfig.algSSamplePhaseDuration = 0x18U;
    selftestConfig.algCSamplePhaseDuration = 0xA0U;
    selftestConfig.baudRate                = 0x00U;
    ADC_SetSelfTestConfig(DEMO_ADC_BASE, &selftestConfig);

    /* Enable self-test watchdog. */
    ADC_EnableSelfTestWdgThreshold(DEMO_ADC_BASE, kADC_SelfTestWdgThresholdForAlgSStep0);
    ADC_EnableSelfTestWdgThreshold(DEMO_ADC_BASE, kADC_SelfTestWdgThresholdForAlgSStep1Integer);
    ADC_EnableSelfTestWdgThreshold(DEMO_ADC_BASE, kADC_SelfTestWdgThresholdForAlgSStep2);
    ADC_EnableSelfTestWdgThreshold(DEMO_ADC_BASE, kADC_SelfTestWdgThresholdForAlgCStep0);

    /* Configure and enable the self-test watchdog timer. */
    ADC_SetSelfTestWdgTimerVal(DEMO_ADC_BASE, kADC_SelfTestWdgTimerVal5);
    ADC_EnableSelfTestWdgTimer(DEMO_ADC_BASE, kADC_SelfTestForAlgSAndC);

    /* Enable the self-test. */
    ADC_EnableSelfTest(DEMO_ADC_BASE);

    /* Enable interrupts. */
    ADC_EnableSelfTestInt(DEMO_ADC_BASE, kADC_AlgSStep0ErrIntEnable | kADC_AlgSStep1ErrIntEnable |
                                             kADC_AlgSStep2ErrIntEnable | kADC_AlgCErrIntEnable |
                                             kADC_WdgTimeErrIntEnable | kADC_WdgSequenceErrIntEnable);
    EnableIRQ(DEMO_ADC_IRQn);

    /* Enable the ADC conversion chain to execute the conversion. */
    ADC_StartConvChain(DEMO_ADC_BASE, kADC_NormalConvScanMode);

    while (1)
    {
        if (ADC_GetChannelConvResult(DEMO_ADC_BASE, &convResult, DEMO_ADC_CHANNEL0))
        {
            convChainRoundCount++; /* Roughly record how many rounds of transformations have gone through. */
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

        /* When the conversion is more than 3 rounds and the current conversion chain is in test channel conversion,
           abort the current conversion chain to make a watchdog sequence error. */
        if ((3U < convChainRoundCount) && ADC_CheckSelfTestConvInProcess(DEMO_ADC_BASE))
        {
            ADC_AbortCurrentConvChain(DEMO_ADC_BASE);
        }

        if (thrsholdIntFlag)
        {
            PRINTF("\r\n Self-test error occurred!");
            DEMO_ADC_SelfTestHandler();
            break;
        }
    }

    while (1)
    {
    }
}

static void DEMO_ADC_SelfTestHandler(void)
{
    uint16_t errData0 = 0U;
    uint16_t errData1 = 0U;

    /* Algorithm S Step 0 error occurred. */
    if (kADC_AlgSStep0ErrIntFlag == (intStatus & kADC_AlgSStep0ErrIntFlag))
    {
        errData0 = ADC_GetSelfTestChannelConvFailedData(DEMO_ADC_BASE, kADC_SelfTestWdgThresholdForAlgSStep0);
        PRINTF("\r\n ADC self test algorithm S Step 0 error occurred, the test channel convert data is %d.", errData0);
    }

    /* Algorithm S Step 1 error occurred. */
    if (kADC_AlgSStep1ErrIntFlag == (intStatus & kADC_AlgSStep1ErrIntFlag))
    {
        errData0 = ADC_GetSelfTestChannelConvFailedData(DEMO_ADC_BASE, kADC_SelfTestWdgThresholdForAlgSStep1Integer);
        errData1 = ADC_GetSelfTestChannelConvFailedData(DEMO_ADC_BASE, kADC_SelfTestWdgThresholdForAlgSStep1Fraction);
        PRINTF(
            "\r\n ADC self-test algorithm S Step 1 error occurred, the integer part value is %d, the fraction part \
                     value is %d.",
            errData0, errData1);
    }

    /* Algorithm S Step 2 error occurred. */
    if (kADC_AlgSStep2ErrIntFlag == (intStatus & kADC_AlgSStep2ErrIntFlag))
    {
        errData0 = ADC_GetSelfTestChannelConvFailedData(DEMO_ADC_BASE, kADC_SelfTestWdgThresholdForAlgSStep2);
        PRINTF("\r\n ADC self test algorithm S Step 2 error occurred, the test channel convert data is %d.", errData0);
    }

    /* Algorithm C error occurred. */
    if (kADC_AlgCErrIntFlag == (intStatus & kADC_AlgCErrIntFlag))
    {
        errData0 = ADC_GetSelfTestChannelConvFailedData(DEMO_ADC_BASE, kADC_SelfTestWdgThresholdForAlgCStep0);
        PRINTF("\r\n ADC self test algorithm C error occurred, the test channel convert data is %d.", errData0);
    }

    /* Watchdog timer error occurred. */
    if (kADC_WdgTimeErrIntFlag == (intStatus & kADC_WdgTimeErrIntFlag))
    {
        PRINTF("\r\n ADC self-test watchdog timer error occurred.");
    }

    /* Watchdog sequence error occurred. */
    if (kADC_WdgSequenceErrIntFlag == (intStatus & kADC_WdgSequenceErrIntFlag))
    {
        PRINTF("\r\n ADC self-test watchdog sequence error occurred.");
    }
}
