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

#include "fsl_tpm.h"
#include "fsl_xbar.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PWM_FREQUENCY (24000U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
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

void tpm_init(void)
{
    tpm_config_t tpmInfo;
    tpm_chnl_pwm_signal_param_t tpmParam;
    int updatedDutycycle = 5;
    uint8_t control;

    TPM_GetDefaultConfig(&tpmInfo);
    tpmInfo.prescale = TPM_CalculateCounterClkDiv(BOARD_TPM_BASEADDR, DEMO_PWM_FREQUENCY, TPM_SOURCE_CLOCK);
    TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);
    tpmParam.chnlNumber = (tpm_chnl_t)BOARD_TPM_CHANNEL;
    tpmParam.level            = kTPM_HighTrue;
    tpmParam.dutyCyclePercent = updatedDutycycle;
    if (kStatus_Success != TPM_SetupPwm(BOARD_TPM_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, DEMO_PWM_FREQUENCY, TPM_SOURCE_CLOCK)) {
	    PRINTF("\r\nSetup PWM fail!\r\n");
	    return;
    }
    TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);
    control = TPM_GetChannelContorlBits(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL);
    TPM_EnableChannel(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, control);
    TPM_EnableInterrupts(BOARD_TPM_BASEADDR, kTPM_TimeOverflowInterruptEnable);
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

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    XBAR_Init(kXBAR_DSC1);
    XBAR_SetSignalsConnection(kXBAR1_InputTpm6LptpmChTrigger0, kXBAR1_OutputAnamixGlueTrgmuxStartTrg);
    PRINTF("\r\nIPSYNC trigger signal connected! \r\n");

    tpm_init();

    PRINTF("\r\n ADC polling example.");

    /* Configure the ADC. */
    ADC_GetDefaultConfig(&adcConfig);
    adcConfig.extTrig = kADC_ExtTrigFallingEdge;
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
    }
}
