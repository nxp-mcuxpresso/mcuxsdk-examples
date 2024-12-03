/*
 * Copyright 2024 NXP
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
static void DEMO_DoLpadcConfig(ADC_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    lpadc_conv_result_t lpadcResultConfigStruct;

    uint32_t opamp0OutputValue            = 0U;
    uint32_t opamp1OutputValue            = 0U;
    uint32_t opamp1PositiveInputValue     = 0U;
    uint32_t opamp1OutputValueTheoretical = 0U;

    BOARD_InitHardware();

    DEMO_DoOpampConfig();
    DEMO_DoLpadcConfig(DEMO_LPADC_INSTANCE0);
    DEMO_DoLpadcConfig(DEMO_LPADC_INSTANCE1);

    OPAMP_Enable(DEMO_OPAMP_INSTANCE0, true);
    OPAMP_Enable(DEMO_OPAMP_INSTANCE1, true);

    PRINTF("\r\n Opamp lpadc example.");
    PRINTF("\r\n Please press any key to get opamp output value.");

    while (1)
    {
        GETCHAR();

        LPADC_DoSoftwareTrigger(DEMO_LPADC_INSTANCE0, (1U << DEMO_LPADC_OPAMP0_OUTPUT_TRIGGERID));

#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2U))
        while (!LPADC_GetConvResult(DEMO_LPADC_INSTANCE0, &lpadcResultConfigStruct, 0U))
#else
        while (!LPADC_GetConvResult(DEMO_LPADC_INSTANCE0, &lpadcResultConfigStruct))
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */
        {
        }

        /* OPAMP0 output. */
        if (lpadcResultConfigStruct.commandIdSource == DEMO_LPADC_OPAMP0_OUTPUT_CMDID)
        {
            opamp0OutputValue =
                (uint32_t)((float)(DEMO_LPADC_VREF_VALUE * (lpadcResultConfigStruct.convValue >> lpadcResultShift)) /
                           (float)lpadcFullRange);

            PRINTF("\r\n opamp0OutputValue = %dmv", opamp0OutputValue);
        }

#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2U))
        while (!LPADC_GetConvResult(DEMO_LPADC_INSTANCE0, &lpadcResultConfigStruct, 0U))
#else
        while (!LPADC_GetConvResult(DEMO_LPADC_INSTANCE0, &lpadcResultConfigStruct))
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */
        {
        }

        /* OPAMP1 positive input. */
        if (lpadcResultConfigStruct.commandIdSource == DEMO_LPADC_OPAMP1_POSITIVE_INPUT_CMDID)
        {
            opamp1PositiveInputValue =
                (uint32_t)((float)(DEMO_LPADC_VREF_VALUE * (lpadcResultConfigStruct.convValue >> lpadcResultShift)) /
                           (float)lpadcFullRange);

            PRINTF("\r\n opamp1PositiveInputValue = %dmv", opamp1PositiveInputValue);
        }

        LPADC_DoSoftwareTrigger(DEMO_LPADC_INSTANCE1, (1U << DEMO_LPADC_OPAMP1_OUTPUT_TRIGGERID));

#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2U))
        while (!LPADC_GetConvResult(DEMO_LPADC_INSTANCE1, &lpadcResultConfigStruct, 0U))
#else
        while (!LPADC_GetConvResult(DEMO_LPADC_INSTANCE1, &lpadcResultConfigStruct))
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */
        {
        }

        /* OPAMP1 output */
        opamp1OutputValue =
            (uint32_t)((float)(DEMO_LPADC_VREF_VALUE * (lpadcResultConfigStruct.convValue >> lpadcResultShift)) /
                       (float)lpadcFullRange);
        opamp1OutputValueTheoretical =
            opamp0OutputValue +
            (uint32_t)((float)(DEMO_OPAMP_RESISTOR_2 * (opamp1PositiveInputValue - DEMO_OPAMP_NEGATIVE_VALUE)) /
                       (float)DEMO_OPAMP_RESISTOR_1);

        PRINTF("\r\n Opamp1 output value measured by lpadc: %dmv", opamp1OutputValue);
        PRINTF("\r\n Opamp1 theoretical output value: %dmv", opamp1OutputValueTheoretical);
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
    OPAMP_Init(DEMO_OPAMP_INSTANCE1, &config);
}

/*! @brief This function is used to configure LPADC. */
static void DEMO_DoLpadcConfig(ADC_Type *base)
{
    lpadc_config_t lpadcConfigStruct;
    lpadc_conv_trigger_config_t lpadcTriggerConfigStruct;
    lpadc_conv_command_config_t lpadcCommandConfigStruct;

    LPADC_GetDefaultConfig(&lpadcConfigStruct);
    lpadcConfigStruct.enableAnalogPreliminary = DEMO_LPADC_ENABLE_ANALOG_PRELIMINARY;
    lpadcConfigStruct.referenceVoltageSource  = DEMO_LPADC_VREF_SOURCE;
#if (defined(FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS) && FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS)
    lpadcConfigStruct.conversionAverageMode = DEMO_LPADC_CONV_AVGS_MODE;
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS */
    LPADC_Init(base, &lpadcConfigStruct);

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFS) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFS
    LPADC_DoOffsetCalibration(base);
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFS */

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ) && FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ
    LPADC_DoAutoCalibration(base);
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ */

    LPADC_GetDefaultConvCommandConfig(&lpadcCommandConfigStruct);
    lpadcCommandConfigStruct.sampleChannelMode   = DEMO_LPADC_SAMPLE_CHANNEL_MODE;
    lpadcCommandConfigStruct.hardwareAverageMode = DEMO_LPADC_HARDWARE_AVERAGE_MODE;
#if (defined(FSL_FEATURE_LPADC_HAS_CMDL_MODE) && FSL_FEATURE_LPADC_HAS_CMDL_MODE)
    lpadcCommandConfigStruct.conversionResolutionMode = DEMO_LPADC_CONVERSION_RESOLUTION_MODE;
#endif /* FSL_FEATURE_LPADC_HAS_CMDL_MODE */
    if (base == DEMO_LPADC_INSTANCE0)
    {
        lpadcCommandConfigStruct.channelNumber            = DEMO_LPADC_OPAMP0_OUTPUT_CHANNEL;
        lpadcCommandConfigStruct.chainedNextCommandNumber = DEMO_LPADC_OPAMP1_POSITIVE_INPUT_CMDID;
        LPADC_SetConvCommandConfig(base, DEMO_LPADC_OPAMP0_OUTPUT_CMDID, &lpadcCommandConfigStruct);

        LPADC_GetDefaultConvCommandConfig(&lpadcCommandConfigStruct);
        lpadcCommandConfigStruct.sampleChannelMode   = DEMO_LPADC_SAMPLE_CHANNEL_MODE;
        lpadcCommandConfigStruct.hardwareAverageMode = DEMO_LPADC_HARDWARE_AVERAGE_MODE;
#if (defined(FSL_FEATURE_LPADC_HAS_CMDL_MODE) && FSL_FEATURE_LPADC_HAS_CMDL_MODE)
        lpadcCommandConfigStruct.conversionResolutionMode = DEMO_LPADC_CONVERSION_RESOLUTION_MODE;
#endif /* FSL_FEATURE_LPADC_HAS_CMDL_MODE */
        lpadcCommandConfigStruct.channelNumber = DEMO_LPADC_OPAMP1_POSITIVE_INPUT_CHANNEL;

        LPADC_SetConvCommandConfig(base, DEMO_LPADC_OPAMP1_POSITIVE_INPUT_CMDID, &lpadcCommandConfigStruct);
    }
    else
    {
        lpadcCommandConfigStruct.channelNumber = DEMO_LPADC_OPAMP1_OUTPUT_CHANNEL;
        LPADC_SetConvCommandConfig(base, DEMO_LPADC_OPAMP1_OUTPUT_CMDID, &lpadcCommandConfigStruct);
    }

    LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfigStruct);
    lpadcTriggerConfigStruct.enableHardwareTrigger = DEMO_LPADC_ENABLE_HARDWARE_TRIGGER;
    if (base == DEMO_LPADC_INSTANCE0)
    {
        lpadcTriggerConfigStruct.targetCommandId = DEMO_LPADC_OPAMP0_OUTPUT_CMDID;
        LPADC_SetConvTriggerConfig(base, DEMO_LPADC_OPAMP0_OUTPUT_TRIGGERID, &lpadcTriggerConfigStruct);
    }
    else
    {
        lpadcTriggerConfigStruct.targetCommandId = DEMO_LPADC_OPAMP1_OUTPUT_CMDID;
        LPADC_SetConvTriggerConfig(base, DEMO_LPADC_OPAMP1_OUTPUT_TRIGGERID, &lpadcTriggerConfigStruct);
    }
}
