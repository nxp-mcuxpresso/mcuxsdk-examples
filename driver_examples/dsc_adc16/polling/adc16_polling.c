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
const uint32_t g_Adc16_12bitFullRange = 4096U;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    adc16_config_t adc16ConfigStruct;
    
    BOARD_InitHardware();

    PRINTF("\r\nADC16 polling Example.\r\n");
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
    adc16ConfigStruct.bEnableInterruptOnConversionCompleted = false;
    
    ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);
    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASE))
    {
        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }

    PRINTF("ADC Full Range: %ld\r\n", g_Adc16_12bitFullRange);
    PRINTF("Press any key to get user channel's ADC value ...\r\n");
    
    while (1)
    {
        GETCHAR();
        
        /*
         When in software trigger mode,each conversion would be launched once calling the "Adc16_DoSoftwareTrigger()"
         function,which works like writing a conversion command and executing it. For another channel's conversion,
         just to change the "DEMO_ADC16_GROUPID" field in channel's configuration structure, and call the
         "Adc16_DoSoftwareTrigger() again.
        */
        Adc16_DoSoftwareTrigger(DEMO_ADC16_BASE,DEMO_ADC16_GROUPID);
        
        while(false == ADC16_IsGroupConversionCompleted(DEMO_ADC16_BASE,DEMO_ADC16_GROUPID))
        {
        	
        }
        PRINTF("ADC Value: %ld\r\n", ADC16_GetGroupConversionValue(DEMO_ADC16_BASE,DEMO_ADC16_GROUPID));
        
    }
}


