/* Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "app.h"
#include "fsl_adc16.h"
#include "fsl_dac.h"
#include "fsl_debug_console.h"
#include "fsl_component_serial_manager.h"
#include "fsl_shell.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SHELL_Printf PRINTF
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void DAC_Configuration(void);

static void ADC_Configuration(void);

static void ADC_GetReferenceVoltage(void);

static shell_status_t SetVoltage(shell_handle_t shellHandle, int32_t argc, char **argv);

static shell_status_t GetVoltage(shell_handle_t shellHandle, int32_t argc, char **argv);

static shell_status_t ResetTargetBoard(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/
DAC_Type *dacBase = DAC0;
float g_VrefValue;
SHELL_COMMAND_DEFINE(setv,
                     "\r\n\"setv arg1 arg2 arg3\":\r\n Usage:\r\n    arg1: 1|1.5|3.3...  "
                     "   output voltage\r\n    arg2: 1.5|1.8|3.3...   target Vref\r\n"
                     "    arg3: DAC0|DAC1        ADC instance\r\n",
                     SetVoltage,
                     3);
SHELL_COMMAND_DEFINE(getv,
                     "\r\n\"getv arg1\":\r\n Usage:\r\n    arg1: 0|1|2|3...       ADC input channel\r\n",
                     GetVoltage,
                     1);
SHELL_COMMAND_DEFINE(reset,
                     "\r\n\"reset\": Reset target board, please connect JP11-4 pin and target board's reset pin\r\n",
                     ResetTargetBoard,
                     0);

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

extern serial_handle_t g_serialHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

static shell_status_t SetVoltage(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    uint32_t dacValue;
    float outputValue  = ((float)atof(argv[1]));
    float targetVref   = ((float)atof(argv[2]));
    char *dacInstatnce = argv[3];

    if (strcmp(dacInstatnce, "DAC0") == 0)
    {
        dacBase = DAC0;
    }
    else if (strcmp(dacInstatnce, "DAC1") == 0)
    {
        dacBase = DAC1;
    }
    else
    {
        SHELL_Printf("Control conmmand is wrong!\r\n");
        return (shell_status_t)-1;
    }

    DAC_Configuration();
    ADC_Configuration();
    ADC_GetReferenceVoltage();

    if (outputValue > targetVref)
    {
        SHELL_Printf("DAC output voltage is larger than target board's Vref.\r\n");
        return (shell_status_t)-2;
    }
    else if (outputValue > g_VrefValue)
    {
        SHELL_Printf("DAC output valtage is larger than self-Vref, self-Vref is %.2fV\r\n", g_VrefValue);
        return (shell_status_t)-3;
    }
    else
    {
        dacValue = (uint32_t)((outputValue / g_VrefValue) * 4095);
        DAC_SetBufferValue(dacBase, 0U, dacValue);
        SHELL_Printf("DAC output is %.2fV, DAC reference voltage is %.2fV\r\n", outputValue, g_VrefValue);
        return (shell_status_t)0;
    }
}

static shell_status_t GetVoltage(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    float inputVoltage;
    int32_t adcChannel = ((int32_t)atoi(argv[1]));
    adc16_channel_config_t adc16ChannelConfigStruct;

    ADC_Configuration();
    ADC_GetReferenceVoltage();

    adc16ChannelConfigStruct.channelNumber                        = adcChannel;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */

    ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
    while (0U ==
           (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)))
    {
    }
    inputVoltage = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP) * g_VrefValue / 4095U;
    SHELL_Printf("Input voltage Value is %.2fV\r\n", inputVoltage);
    return (shell_status_t)0;
}

static shell_status_t ResetTargetBoard(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    gpio_pin_config_t config;

    /* Initialize the gpio to high level outpout. */
    config.outputLogic  = 1U;
    config.pinDirection = kGPIO_DigitalOutput;
    GPIO_PinInit(DEMO_GPIO_BASE, DEMO_GPIO_PORT_INDEX, &config);
    /* Pull down target board's reset pin. */
    GPIO_PinWrite(DEMO_GPIO_BASE, DEMO_GPIO_PORT_INDEX, 0U);
    /* Keep low output for little while to make sure target board is reseted.*/
    for (uint32_t i = 0U; i < 1000U; ++i)
    {
        __ASM("nop");
    }
    /* Pull up target board's reset pin to work with target board. */
    GPIO_PinWrite(DEMO_GPIO_BASE, DEMO_GPIO_PORT_INDEX, 1U);

    return (shell_status_t)0;
}

static void DAC_Configuration(void)
{
    dac_config_t dacConfigStruct;

    /* Configure the DAC. */
    /*
     * dacConfigStruct.referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2;
     * dacConfigStruct.enableLowPowerMode = false;
     */
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(dacBase, &dacConfigStruct);
    DAC_Enable(dacBase, true);             /* Enable output. */
    DAC_SetBufferReadPointer(dacBase, 0U); /* Make sure the read pointer to the start. */
                                           /*
                                            * The buffer is not enabled, so the read pointer can not move automatically. However, the buffer's read pointer
                                            * and itemss can be written manually by user.
                                            */
}

static void ADC_Configuration(void)
{
    adc16_config_t adc16ConfigStruct;

    /*
     * adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
     * adc16ConfigStruct.clockSource = kADC16_ClockSourceAsynchronousClock;
     * adc16ConfigStruct.enableAsynchronousClock = false;
     * adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;
     * adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
     * adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
     * adc16ConfigStruct.enableHighSpeed = false;
     * adc16ConfigStruct.enableLowPower = false;
     * adc16ConfigStruct.enableContinuousConversion = false;
     */
    ADC16_GetDefaultConfig(&adc16ConfigStruct);
#ifdef BOARD_ADC_USE_ALT_VREF
    adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
#endif
    ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASE, false); /* Make sure the software trigger is used. */
#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASE))
    {
        SHELL_Printf("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        SHELL_Printf("ADC16_DoAutoCalibration() Failed.\r\n");
    }
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
}

static void ADC_GetReferenceVoltage(void)
{
    float bandgapValue, VrefValue;
    adc16_channel_config_t adc16ChannelConfigStruct;

    /* Prior to reading from this ADC channel, ensure that you
     *  enable the bandgap buffer by setting the PMC_REGSC[BGBE] bit.
     */
    PMC->REGSC |= PMC_REGSC_BGBE_MASK;

    /* Get bandgap voltage value. */
    adc16ChannelConfigStruct.channelNumber                        = DEMO_ADC16_BANDGAP_CHANNEL;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */
    ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
    while (0U ==
           (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)))
    {
    }
    bandgapValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);

    /* Get Vref voltage value. */
    adc16ChannelConfigStruct.channelNumber                        = DEMO_ADC16_VREF_CHANNEL;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */
    ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
    while (0U ==
           (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)))
    {
    }
    VrefValue   = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
    g_VrefValue = VrefValue / bandgapValue;
}

/*! @brief Main function */
int main(void)
{
    BOARD_InitHardware();

    /* Init SHELL */
    s_shellHandle = &s_shellHandleBuffer[0];
    SHELL_Init(s_shellHandle, g_serialHandle, "SHELL>> ");

    /* Add new command to commands list */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(reset));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(setv));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(getv));

    while (1)
    {
#if !(defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
        SHELL_Task(s_shellHandle);
#endif
    }
}
