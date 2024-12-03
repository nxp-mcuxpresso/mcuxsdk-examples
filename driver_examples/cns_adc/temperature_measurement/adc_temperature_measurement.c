/*
 * Copyright 2020, 2022, 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "app.h"
#include "fsl_adc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_CalculateTemperature(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_conversionDataReady = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_ADC_IRQHANDLER(void)
{
    if ((ADC_GetStatusFlags(DEMO_ADC_BASE) & kADC_DataReadyInterruptFlag) != 0UL)
    {
        g_conversionDataReady = true;
        ADC_ClearStatusFlags(DEMO_ADC_BASE, kADC_DataReadyInterruptFlag);
    }
}

int main(void)
{
    adc_config_t adcConfig;

    BOARD_InitHardware();

    PRINTF("\r\nADC Temperature Measurement Example!\r\n");

    /*
     *  config->clockDivider = kADC_ClockDivider1;
     *  config->powerMode = kADC_PowerModeFullBiasingCurrent;
     *  config->resolution = kADC_Resolution12Bit;
     *  config->warmupTime = kADC_WarmUpTime16us;
     *  config->vrefSource = kADC_Vref1P2V;
     *  config->inputMode = kADC_InputSingleEnded;
     *  config->conversionMode = kADC_ConversionContinuous;
     *  config->scanLength = kADC_ScanLength_1;
     *  config->averageLength = kADC_AverageNone;
     *  config->triggerSource = kADC_TriggerSourceSoftware;
     *  config->inputGain = kADC_InputGain1;
     *  config->enableInputGainBuffer = false;
     *  config->resultWidth = kADC_ResultWidth16;
     *  config->fifoThreshold = kADC_FifoThresholdData1;
     *  config->enableInputBufferChop = true;
     *  config->enableDMA = false;
     *  config->enableADC = false;
     */
    ADC_GetDefaultConfig(&adcConfig);
    adcConfig.vrefSource            = kADC_Vref1P2V;
    adcConfig.inputMode             = kADC_InputSingleEnded;
    adcConfig.conversionMode        = kADC_ConversionOneShot;
    adcConfig.inputGain             = kADC_InputGain1;
    adcConfig.resolution            = kADC_Resolution16BitAudio;
    adcConfig.fifoThreshold         = kADC_FifoThresholdData1;
    adcConfig.scanLength            = kADC_ScanLength_1;
    adcConfig.averageLength         = kADC_Average16;
    adcConfig.enableInputGainBuffer = true;
    adcConfig.enableInputBufferChop = false;
    adcConfig.enableADC             = true;
    adcConfig.enableChop            = false;

    ADC_Init(DEMO_ADC_BASE, &adcConfig);
    ADC_EnableTemperatureSensor(DEMO_ADC_BASE, true);
    ADC_SetTemperatureSensorMode(DEMO_ADC_BASE, kADC_TSensorInternal);

    if (ADC_DoAutoCalibration(DEMO_ADC_BASE, kADC_CalibrationVrefInternal) != kStatus_Success)
    {
        PRINTF("\r\nCalibration Failed!\r\n");
        return 0;
    }

    PRINTF("\r\nCalibration Success!\r\n");
    ADC_ClearStatusFlags(DEMO_ADC_BASE, kADC_DataReadyInterruptFlag);
    ADC_SetScanChannel(DEMO_ADC_BASE, kADC_ScanChannel0, kADC_TEMPP);
    ADC_EnableInterrupts(DEMO_ADC_BASE, kADC_DataReadyInterruptEnable);
    EnableIRQ(DEMO_ADC_IRQn);
    while (1)
    {
        PRINTF("Please press any key to get internal diode temperature.\r\n");
        GETCHAR();
        ADC_DoSoftwareTrigger(DEMO_ADC_BASE);
        while (!g_conversionDataReady)
            ;
        DEMO_CalculateTemperature();
        g_conversionDataReady = false;
        ADC_StopConversion(DEMO_ADC_BASE);
    }
}

/*!
 * @brief Calculate Temperature based on the formula:
 *              Tmeans(in C) = Conversion Result/TS_GAIN - TS_OFFSET.
 */
static void DEMO_CalculateTemperature(void)
{
    float temperature = 0.0f;
    int16_t conversionResult;

    conversionResult = (int16_t)((int32_t)ADC_GetConversionResult(DEMO_ADC_BASE));
    temperature      = (float)conversionResult / DEMO_ADC_TS_GAIN;
    temperature -= DEMO_ADC_TS_OFFSET;
    PRINTF("\r\nTemperature: %6.3f Celsius.\r\n", (double)temperature);
}
