/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_adc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_Adc16ConversionDoneFlag = false;
volatile uint32_t g_Adc16ConversionValue;
volatile uint32_t g_Adc16InterruptCounter;
const uint32_t g_Adc16_12bitFullRange = 4096U;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief ISR for ADC16 interrupt function
 */
#pragma interrupt alignsp saveall
void DEMO_ADC16_IRQHandler(void)
{
    g_Adc16ConversionDoneFlag = true;
    /* Read conversion result to clear the conversion completed flag. */
    g_Adc16ConversionValue = ADC16_GetGroupConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_GROUPID);
    g_Adc16InterruptCounter++;
}
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    adc16_config_t adc16ConfigStruct;

    BOARD_InitHardware();

    PRINTF("\r\nADC16 interrupt Example.\r\n");

    /*
     * adc16ConfigStruct.eReferenceVoltageSource                  = kADC16_ReferenceVoltageSourceVref;
     * adc16ConfigStruct.eClockSource                             = kADC16_ClockSourceAsynchronousClock;
     * adc16ConfigStruct.bEnableAsynchronousClock                 = true;
     * adc16ConfigStruct.eClockDivider                            = kADC16_ClockDivider8;
     * adc16ConfigStruct.eResolution                              = kADC16_Resolution12bit;
     * adc16ConfigStruct.eSampleMode                              = kADC16_SampleDisabled;
     * adc16ConfigStruct.bEnableHighSpeed                         = false;
     * adc16ConfigStruct.bEnableLowPower                          = false;
     * adc16ConfigStruct.eChannelInput                            = kADC16_ModuleDisabled;
     * adc16ConfigStruct.u32GroupId                               = DEMO_ADC16_GROUPID;
     * adc16ConfigStruct.bEnableInterruptOnConversionCompleted    = false;
     * adc16ConfigStruct.bEnableDMA                               = false;
     * adc16ConfigStruct.eConversionTrigger                       = kADC16_ConversionTrggerDisable;
     * adc16ConfigStruct.eHardwareAverageMode                     = kADC16_HardwareAverageDisabled;
     * adc16ConfigStruct.bEnableContinuousConversion              = false;
     * adc16ConfigStruct.eCompareMode                             = NULL;
     */

    ADC16_GetDefaultConfig(&adc16ConfigStruct);

    adc16ConfigStruct.eReferenceVoltageSource               = kADC16_ReferenceVoltageSourceVref;
    adc16ConfigStruct.eChannelInput                         = kADC16_Input17Channel;
    adc16ConfigStruct.u32GroupId                            = DEMO_ADC16_GROUPID;
    adc16ConfigStruct.bEnableInterruptOnConversionCompleted = true;

    ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);

    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASE))
    {
        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }

    EnableIRQWithPriority(DEMO_ADC16_IRQ_ID, 1);

    PRINTF("ADC Full Range: %ld\r\n", g_Adc16_12bitFullRange);
    PRINTF("Press any key to get user channel's ADC value ...\r\n");

    g_Adc16InterruptCounter = 0U;

    while (1)
    {
        GETCHAR();
        g_Adc16ConversionDoneFlag = false;
        /*
              When in the software trigger mode, each conversion will call the "Adc16_DoSoftwareTrigger()" function to
           start once, its job is similar to writing a conversion command and executing it. For the conversion of
           another channel, just modify the "DEMO_ADC16_GROUPID" field in the channel configuration structure and call
           this function "Adc16_DoSoftwareTrigger()" again. In addition, the "enableInterruptOnConversionCompleted" in
           the channel configuration the structure is the parameter of the interrupt command. Only need to start the
           interrupt conversion once, no need the enable of the interrupt parameter is executed every time.
        */
        Adc16_DoSoftwareTrigger(DEMO_ADC16_BASE, DEMO_ADC16_GROUPID);
        while (!g_Adc16ConversionDoneFlag)
        {
        }
        PRINTF("ADC Value: %ld\r\n", g_Adc16ConversionValue);
        PRINTF("ADC Interrupt Count: %ld\r\n", g_Adc16InterruptCounter);
    }
}
