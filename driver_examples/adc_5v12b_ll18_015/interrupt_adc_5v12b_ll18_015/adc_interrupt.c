/*
 * Copyright 2017-2018, 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_adc.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_AdcConversionDoneFlag;
volatile uint32_t g_AdcConversionValue;
volatile uint32_t g_AdcInterruptCounter;
const uint32_t g_Adc_8bitFullRange = 256U;

/*******************************************************************************
 * Code
 ******************************************************************************/

void EXAMPLE_ADC_IRQHandler(void)
{
    g_AdcConversionDoneFlag = true;
    /* Read conversion result to clear the conversion completed flag. */
    g_AdcConversionValue = ADC_GetChannelConversionValue(DEMO_ADC_BASE);
    g_AdcInterruptCounter++;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    adc_config_t adcConfigStruct;
    adc_channel_config_t adcChannelConfigStruct;

    BOARD_InitHardware();
    EnableIRQ(DEMO_ADC_IRQn);

    PRINTF("\r\nADC interrupt Example.\r\n");

    /*
     * config->referenceVoltageSource = kADC_ReferenceVoltageSourceAlt0;
     * config->enableLowPower = false;
     * config->enableLongSampleTime = false;
     * config->clockDivider = kADC_ClockDivider1;
     * config->ResolutionMode = kADC_Resolution8BitMode;
     * config->clockSource = kADC_ClockSourceAlt0;
     */
    ADC_GetDefaultConfig(&adcConfigStruct);
    ADC_Init(DEMO_ADC_BASE, &adcConfigStruct);
    ADC_EnableHardwareTrigger(DEMO_ADC_BASE, false);

    /* Configure the user channel and interrupt. */
    adcChannelConfigStruct.channelNumber                        = DEMO_ADC_USER_CHANNEL;
    adcChannelConfigStruct.enableInterruptOnConversionCompleted = true;
    adcChannelConfigStruct.enableContinuousConversion           = false;
    g_AdcInterruptCounter                                       = 0U; /* Clear the interrupt counter. */

    /* Enable the releated analog pins. */
    ADC_EnableAnalogInput(DEMO_ADC_BASE, 1U << DEMO_ADC_USER_CHANNEL, true);

    PRINTF("ADC Full Range: %d\r\n", g_Adc_8bitFullRange);
    PRINTF("Press any key to get user channel's ADC value.\r\n");
    while (1)
    {
        GETCHAR();
        g_AdcConversionDoneFlag = false;
        /*
         * When in software trigger mode, each conversion would be launched once calling the "ADC_ChannelConfigure()"
         * function, which works like writing a conversion command and executing it. For another channel's conversion,
         * just to change the "channelNumber" field in channel configuration structure, and call the function
         * "ADC_ChannelConfigure()" again.
         * Also, the "enableInterruptOnConversionCompleted" inside the channel configuration structure is a parameter
         * for the conversion command. It takes affect just for the current conversion. If the interrupt is still
         * required for the following conversion, it is necessary to assert the "enableInterruptOnConversionCompleted"
         * every time for each command.
         */
        ADC_SetChannelConfig(DEMO_ADC_BASE, &adcChannelConfigStruct);
        while (!g_AdcConversionDoneFlag)
        {
        }
        PRINTF("ADC Value: %d\r\n", g_AdcConversionValue);
        PRINTF("ADC Interrupt Counter: %d\r\n", g_AdcInterruptCounter);
    }
}
