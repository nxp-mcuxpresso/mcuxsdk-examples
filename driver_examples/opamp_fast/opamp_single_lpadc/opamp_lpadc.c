/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_opamp.h"
#include "fsl_lpadc.h"
#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if (defined(DEMO_LPADC_USE_HIGH_RESOLUTION) && DEMO_LPADC_USE_HIGH_RESOLUTION)
const uint32_t lpadcFullRange   = 65535U;
const uint32_t lpadcResultShift = 0U;
#else
const uint32_t lpadcFullRange   = 4095U;
const uint32_t lpadcResultShift = 3U;
#endif /* DEMO_LPADC_USE_HIGH_RESOLUTION */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_DoOpampConfig(void);
static void DEMO_DoLpadcConfig();

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    lpadc_conv_result_t lpadcResultConfigStruct;

    float opamp0OutputValue            = 0.0F;
    float opamp0NegativeInputValue     = 0.0F;
    float opamp0OutputValueTheoretical = 0.0F;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    DEMO_DoOpampConfig();
    DEMO_DoLpadcConfig();

    OPAMP_Enable(DEMO_OPAMP_INSTANCE0, true);

    PRINTF("\r\n Opamp lpadc example.");
    PRINTF("\r\n Please press any key to get opamp output value.");

    while (1)
    {
        GETCHAR();

        LPADC_DoSoftwareTrigger(DEMO_LPADC_INSTANCE0, (1U << DEMO_LPADC_OPAMP_OUTPUT_TRIGGERID));

        while (!LPADC_GetConvResult(DEMO_LPADC_INSTANCE0, &lpadcResultConfigStruct))
        {
        }
        if (lpadcResultConfigStruct.commandIdSource == DEMO_LPADC_OPAMP_OUTPUT_CMDID)
        {
            opamp0OutputValue = ((float)(DEMO_LPADC_VREF_VALUE * (lpadcResultConfigStruct.convValue >> lpadcResultShift)) / 
                                 (float)lpadcFullRange);
        }
        PRINTF("\r\n Opamp output voltage = %d mv", (uint32_t)opamp0OutputValue);

        while (!LPADC_GetConvResult(DEMO_LPADC_INSTANCE0, &lpadcResultConfigStruct))
        {
        }
        if (lpadcResultConfigStruct.commandIdSource == DEMO_LPADC_OPAMP_NEGATIVE_INPUT_CMDID)
        {
            opamp0NegativeInputValue = ((float)(DEMO_LPADC_VREF_VALUE * (lpadcResultConfigStruct.convValue >> lpadcResultShift)) / 
                                        (float)lpadcFullRange);
        }
        PRINTF("\r\n Opamp negative input voltage = %d mv", (uint32_t)opamp0NegativeInputValue);

        opamp0OutputValueTheoretical =  ((DEMO_OPAMP_REFERENCE * (DEMO_OPAMP_RESISTOR_1 + DEMO_OPAMP_RESISTOR_2) /
                                         (DEMO_OPAMP_RESISTOR_1 + DEMO_OPAMP_RESISTOR_3)) -
                                         (opamp0NegativeInputValue * DEMO_OPAMP_RESISTOR_2 / DEMO_OPAMP_RESISTOR_1));

        PRINTF("\r\n Opamp theoretical output value = %d mv", (uint32_t)opamp0OutputValueTheoretical);
    }
}

/*! @brief This function is used to configure OPAMP. */
static void DEMO_DoOpampConfig(void)
{
    opamp_config_t config;

    OPAMP_GetDefaultConfig(&config);
    config.compCap     = DEMO_OPAMP_COMP_CAP;
    config.biasCurrent = DEMO_OPAMP_BIAS_CURRENT;
    OPAMP_Init(DEMO_OPAMP_INSTANCE0, &config);
}

/*! @brief This function is used to configure LPADC. */
static void DEMO_DoLpadcConfig()
{
    lpadc_config_t lpadcConfigStruct;
    lpadc_conv_trigger_config_t lpadcTriggerConfigStruct;
    lpadc_conv_command_config_t lpadcCommandConfigStruct;

    LPADC_GetDefaultConfig(&lpadcConfigStruct);
    lpadcConfigStruct.enableAnalogPreliminary = DEMO_LPADC_ENABLE_ANALOG_PRELIMINARY;
    lpadcConfigStruct.referenceVoltageSource  = DEMO_LPADC_VREF_SOURCE;
    lpadcConfigStruct.powerLevelMode          = DEMO_LPADC_POWER_MODE;
#if (defined(FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS) && FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS)
    lpadcConfigStruct.conversionAverageMode = DEMO_LPADC_CONV_AVGS_MODE;
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS */
    LPADC_Init(DEMO_LPADC_INSTANCE0, &lpadcConfigStruct);

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFS) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFS
    LPADC_DoOffsetCalibration(DEMO_LPADC_INSTANCE0);
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFS */

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ) && FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ
    LPADC_DoAutoCalibration(DEMO_LPADC_INSTANCE0);
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ */

    LPADC_GetDefaultConvCommandConfig(&lpadcCommandConfigStruct);
    lpadcCommandConfigStruct.sampleChannelMode          = DEMO_LPADC_SAMPLE_CHANNEL_MODE;
    lpadcCommandConfigStruct.hardwareAverageMode        = DEMO_LPADC_HARDWARE_AVERAGE_MODE;
    lpadcCommandConfigStruct.sampleTimeMode             = DEMO_LPADC_SAMPLE_TIME_MODE;
    lpadcCommandConfigStruct.channelNumber              = DEMO_LPADC_OPAMP_OUTPUT_CHANNEL;
    lpadcCommandConfigStruct.chainedNextCommandNumber   = DEMO_LPADC_OPAMP_NEGATIVE_INPUT_CMDID;
#if (defined(FSL_FEATURE_LPADC_HAS_CMDL_MODE) && FSL_FEATURE_LPADC_HAS_CMDL_MODE)
    lpadcCommandConfigStruct.conversionResolutionMode   = DEMO_LPADC_CONVERSION_RESOLUTION_MODE;
#endif /* FSL_FEATURE_LPADC_HAS_CMDL_MODE */
    LPADC_SetConvCommandConfig(DEMO_LPADC_INSTANCE0, DEMO_LPADC_OPAMP_OUTPUT_CMDID, &lpadcCommandConfigStruct);

    LPADC_GetDefaultConvCommandConfig(&lpadcCommandConfigStruct);
    lpadcCommandConfigStruct.sampleChannelMode          = DEMO_LPADC_SAMPLE_CHANNEL_MODE;
    lpadcCommandConfigStruct.hardwareAverageMode        = DEMO_LPADC_HARDWARE_AVERAGE_MODE;
    lpadcCommandConfigStruct.sampleTimeMode             = DEMO_LPADC_SAMPLE_TIME_MODE;
    lpadcCommandConfigStruct.channelNumber              = DEMO_LPADC_OPAMP_NEGATIVE_INPUT_CHANNEL;
#if (defined(FSL_FEATURE_LPADC_HAS_CMDL_MODE) && FSL_FEATURE_LPADC_HAS_CMDL_MODE)
    lpadcCommandConfigStruct.conversionResolutionMode = DEMO_LPADC_CONVERSION_RESOLUTION_MODE;
#endif /* FSL_FEATURE_LPADC_HAS_CMDL_MODE */
    LPADC_SetConvCommandConfig(DEMO_LPADC_INSTANCE0, DEMO_LPADC_OPAMP_NEGATIVE_INPUT_CMDID, &lpadcCommandConfigStruct);

    LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfigStruct);
    lpadcTriggerConfigStruct.enableHardwareTrigger = DEMO_LPADC_ENABLE_HARDWARE_TRIGGER;
    lpadcTriggerConfigStruct.targetCommandId = DEMO_LPADC_OPAMP_OUTPUT_CMDID;
    LPADC_SetConvTriggerConfig(DEMO_LPADC_INSTANCE0, DEMO_LPADC_OPAMP_OUTPUT_TRIGGERID, &lpadcTriggerConfigStruct);
}
