/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_adc.h"
#include "fsl_dac.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DAC_1_0_VOLTS 1241U
#define DAC_1_5_VOLTS 1862U
#define DAC_2_0_VOLTS 2482U
#define DAC_2_5_VOLTS 3103U
#define DAC_3_0_VOLTS 3724U

#define VREF_BRD 3.300
#define SE_12BIT 4096.0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Initialize ADC & DAC. */
static void DAC_ADC_Init(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_AdcConversionValue = 0;
adc_channel_config_t g_adcChannelConfigStruct;
const uint32_t g_Adc_12bitFullRange = 4096U;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void DAC_ADC_Init(void)
{
    adc_config_t adcConfigStruct;
    dac_config_t dacConfigStruct;

    /* Configure the DAC. */
    /*
     * dacConfigStruct.fifoWatermarkLevel = 0U;
     * dacConfigStruct.fifoTriggerMode = kDAC_FIFOTriggerByHardwareMode;
     * dacConfigStruct.fifoWorkMode = kDAC_FIFODisabled;
     * dacConfigStruct.enableLowPowerMode = false;
     * dacConfigStruct.referenceVoltageSource = kDAC_ReferenceVoltageSourceAlt1;
     */
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASE, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASE, true); /* Enable the logic and output. */

    /* Configure the ADC. */
    /*
     * adcConfigStruct.enableAsynchronousClockOutput = true;
     * adcConfigStruct.enableOverWrite = false;
     * adcConfigStruct.enableContinuousConversion = false;
     * adcConfigStruct.enableHighSpeed = false;
     * adcConfigStruct.enableLowPower = false;
     * adcConfigStruct.enableLongSample = false;
     * adcConfigStruct.referenceVoltageSource = kADC_ReferenceVoltageSourceAlt0;
     * adcConfigStruct.samplePeriodMode = kADC_SamplePeriod2or12Clocks;
     * adcConfigStruct.clockSource = kADC_ClockSourceAD;
     * adcConfigStruct.clockDriver = kADC_ClockDriver1;
     * adcConfigStruct.resolution = kADC_Resolution12Bit;
     */
    ADC_GetDefaultConfig(&adcConfigStruct);
    ADC_Init(DEMO_ADC_BASE, &adcConfigStruct);
#if !(defined(FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE) && FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE)
    ADC_EnableHardwareTrigger(DEMO_ADC_BASE, false);
#endif
    if (kStatus_Success == ADC_DoAutoCalibration(DEMO_ADC_BASE))
    {
        PRINTF("Auto calibration is successful!\r\n");
    }
    else
    {
        PRINTF("Auto calibration is failed!\r\n");
    }

    g_adcChannelConfigStruct.channelNumber                        = DEMO_ADC_USER_CHANNEL;
    g_adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i;
    uint8_t ch = ' ';
    float voltRead;

    BOARD_InitHardware();
    PRINTF("\r\nDAC ADC Demo!\r\n");
    PRINTF("\r\nPress any key to start demo.\r\n");
    GETCHAR();
    PRINTF("\r\nDemo begin...\r\n");
    DAC_ADC_Init();

    PRINTF("ADC Full Range: %d\r\n", g_Adc_12bitFullRange);
    while (1)
    {
        PRINTF(
            "\r\n\r\nSelect DAC output level:\r\n\t1. 1.0 V\r\n\t2. 1.5 V\r\n\t3. 2.0 V\r\n\t4. 2.5 V\r\n\t5. 3.0 "
            "V\r\n-->");
        ch = ' ';
        ch = GETCHAR();
        PUTCHAR(ch);
        switch (ch)
        {
            case '1':
                DAC_SetData(DEMO_DAC_BASE, DAC_1_0_VOLTS);
                break;
            case '2':
                DAC_SetData(DEMO_DAC_BASE, DAC_1_5_VOLTS);
                break;
            case '3':
                DAC_SetData(DEMO_DAC_BASE, DAC_2_0_VOLTS);
                break;
            case '4':
                DAC_SetData(DEMO_DAC_BASE, DAC_2_5_VOLTS);
                break;
            case '5':
                DAC_SetData(DEMO_DAC_BASE, DAC_3_0_VOLTS);
                break;
            default:
                PRINTF("\r\nPlease input a valid number: 1-5 \r\n");
                continue;
                break;
        }

        /* Delay to wait for stable DAC voltage. */
        for (i = 0U; i < 10000U; ++i)
        {
            __ASM("NOP");
        }
        ADC_SetChannelConfig(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP, &g_adcChannelConfigStruct);
        while (0U == ADC_GetChannelStatusFlags(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP))
        {
        }
        g_AdcConversionValue = ADC_GetChannelConversionValue(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP);
        PRINTF("\r\n\r\nADC Value: %d\r\n", g_AdcConversionValue);

        /* Convert ADC value to a voltage based on 3.3V VREFH on board */
        voltRead = (float)(g_AdcConversionValue * (VREF_BRD / SE_12BIT));
        PRINTF("\r\nADC Voltage: %0.3f\r\n", voltRead);
    }
}
