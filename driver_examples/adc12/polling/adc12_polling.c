/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_adc12.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
const uint32_t g_Adc12_8bitFullRange = 256U;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    adc12_config_t adc12ConfigStruct;
    adc12_channel_config_t adc12ChannelConfigStruct;

    BOARD_InitHardware();

    PRINTF("\r\nADC12 polling Example.\r\n");

    /* Initialize ADC. */
    ADC12_GetDefaultConfig(&adc12ConfigStruct);
    adc12ConfigStruct.clockSource = DEMO_ADC12_CLOCK_SOURCE;
    ADC12_Init(DEMO_ADC12_BASE, &adc12ConfigStruct);
    /* Set to software trigger mode. */
    ADC12_EnableHardwareTrigger(DEMO_ADC12_BASE, false);

    /* Calibrate ADC. */
    if (kStatus_Success != ADC12_DoAutoCalibration(DEMO_ADC12_BASE))
    {
        PRINTF("ADC calibration failed!\r\n");
    }

    PRINTF("ADC Full Range: %d\r\n", g_Adc12_8bitFullRange);
    /* Trigger the conversion. */
    PRINTF("Press any key to get user channel's ADC value ...\r\n");
    adc12ChannelConfigStruct.channelNumber                        = DEMO_ADC12_USER_CHANNEL;
    adc12ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    while (true)
    {
        GETCHAR();

        /*
         When in software trigger mode, each conversion would be launched once calling the "ADC12_SetChannelConfig()"
         function, which works like writing a conversion command and executing it. For another channel's conversion,
         just to change the "channelNumber" field in channel's configuration structure, and call the
         "ADC12_SetChannelConfig() again.
        */
        ADC12_SetChannelConfig(DEMO_ADC12_BASE, DEMO_ADC12_CHANNEL_GROUP, &adc12ChannelConfigStruct);
        while (0U == (kADC12_ChannelConversionCompletedFlag &
                      ADC12_GetChannelStatusFlags(DEMO_ADC12_BASE, DEMO_ADC12_CHANNEL_GROUP)))
        {
        }
        PRINTF("ADC Value: %d\r\n", ADC12_GetChannelConversionValue(DEMO_ADC12_BASE, DEMO_ADC12_CHANNEL_GROUP));
    }
}
