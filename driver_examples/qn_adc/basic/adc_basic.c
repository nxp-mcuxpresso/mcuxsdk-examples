/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_adc.h"
#include "fsl_power.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t g_AdcVinn;
float g_AdcBandgap;
const uint32_t g_Adc_12bitFullRange = 4096U;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ADC_Configuration(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Power on ADC */
    POWER_EnableADC(true);

    /* Initialize board hardware. */
    BOARD_InitHardware();
    PRINTF("\r\nADC basic example.\r\n");

    /* Configure the converter and work mode. */
    ADC_Configuration();
    PRINTF("Configuration Done.\r\n");
    PRINTF("ADC Full Range: %d\r\n", g_Adc_12bitFullRange);
    PRINTF("Input any character to get demo channel's ADC value ...\r\n");

    /**
     * ADC single mode
     * (Vext - Vinn) / Vref = Vreg / 2^22  ==> Vext = Vreg * Vref / 2^22 + Vinn
     */
    float fresult;
    uint32_t adcConvResult;
    while (1)
    {
        GETCHAR();
        /* Software triger */
        ADC_DoSoftwareTrigger(DEMO_ADC_BASE);
        /* Wait for convert complete */
        while (!(ADC_GetStatusFlags(DEMO_ADC_BASE) & kADC_DataReadyFlag))
        {
        }
        /* Get the result */
        adcConvResult = ADC_GetConversionResult(DEMO_ADC_BASE);

        fresult = ADC_ConversionResult2Mv(DEMO_ADC_BASE, DEMO_ADC_CHANNEL, DEMO_ADC_CFG_IDX, g_AdcBandgap, g_AdcVinn,
                                          adcConvResult);
        PRINTF("Original: 0x%x\t Ch: %d\t Result: %f(mv)\r\n", adcConvResult, DEMO_ADC_CHANNEL, fresult);
    }
}

static void ADC_Configuration(void)
{
    adc_config_t adcConfigStruct;
    adc_sd_config_t adcSdConfigStruct;

    /**
     * Initial ADC to default configuration.
     */
    ADC_GetDefaultConfig(&adcConfigStruct);
    adcConfigStruct.channelEnable = (1U << DEMO_ADC_CHANNEL);
    adcConfigStruct.channelConfig = (DEMO_ADC_CFG_IDX << DEMO_ADC_CHANNEL);
    adcConfigStruct.triggerSource = DEMO_ADC_TRIGGER;
    adcConfigStruct.convMode      = kADC_ConvModeSingle;
    ADC_Init(DEMO_ADC_BASE, &adcConfigStruct);

    /* Initial ADC Sigma Delta(SD) configuration */
    ADC_GetSdDefaultConfig(&adcSdConfigStruct);
    ADC_SetSdConfig(DEMO_ADC_BASE, DEMO_ADC_CFG_IDX, &adcSdConfigStruct);

    /* Bandgap voltage */
    g_AdcBandgap = ADC_GetBandgapCalibrationResult(DEMO_ADC_BASE, DEMO_ADC_CFG_IDX);

    /* Calibration VINN value */
    g_AdcVinn = ADC_GetVinnCalibrationResult(DEMO_ADC_BASE, &adcConfigStruct);

    /* Enable ADC */
    ADC_Enable(DEMO_ADC_BASE, true);
}
