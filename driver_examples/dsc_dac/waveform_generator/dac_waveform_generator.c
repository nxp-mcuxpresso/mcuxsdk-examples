/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_dac.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _dac_waveform_attribute
{
    uint16_t u16StartValue;   /*!< The start value of the waveform. Should be in the range of minimum to maximum. */
    uint16_t u16StepSize;     /*!< The step size to be added to or subtracted from the current value. */
    uint16_t u16MinValue;     /*!< The minimum value is the lower range limit during automatic waveform generation. */
    uint16_t u16MaxValue;     /*!< The maximum value is the upper range limit during automatic waveform generation. */
    uint32_t u32WaveformFreq; /*!< The frequency of the generated waveform. HZ as the unit.  */
    bool bAutoAdjustStep;     /*!< Enable/Disable auto adjust step size.
                                 - \b true If the step size is out of range, to genrate the desired waveform with
                                             with desired frequency and amplitude, the driver will adjust the step size.
                                 - \b false If the step size is out of range, the driver will report this issue.     */
} dac_waveform_attribute_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief The function to get input values.
 */
static uint32_t APP_GetInputValue(void);

/*!
 * @brief Generates sawtooth waveform based on the attributes.
 */
static status_t APP_GenerateSawtoothWaveform(const dac_waveform_attribute_t *psWaveformAttr,
                                             uint32_t u32ClockSourceFreq,
                                             bool bIncrease);
/*!
 * @brief Generates triangle waveform based on the attributes.
 */
static status_t APP_GenerateTriangleWaveform(const dac_waveform_attribute_t *psWaveformAttr,
                                             uint32_t u32ClockSourceFreq,
                                             bool bIncrease);

/*!
 * @brief Gets the waveform attributes from the user inputs.
 */
static void APP_GetWaveformAttribute(dac_waveform_attribute_t *psWaveformAttr);

/*!
 * @brief Sets the waveform attributes.
 */
static status_t APP_SetWaveformAttribute(const dac_waveform_attribute_t *psWaveformAttr,
                                         dac_waveform_type_t eWaveformType,
                                         uint32_t u32ClockSourceFreq);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    dac_config_t sDacConfig;
    dac_waveform_attribute_t sWaveformAttribute;
    char cSawtoothWaveform0InputChar = 'A';
    uint8_t u8Ch;
    dac_waveform_type_t eWaveformType;
    status_t eStatus;

    BOARD_InitHardware();

    PRINTF("\r\n DAC waveform generator Example.\r\n");

    sWaveformAttribute.bAutoAdjustStep = true;
    while (1)
    {
        PRINTF("\r\nPlease select the desired waveform to generate.\r\n");
        PRINTF("Press %c to generate sawtooth waveform0.\r\n", cSawtoothWaveform0InputChar);
        PRINTF("Press %c to generate sawtooth waveform1.\r\n", cSawtoothWaveform0InputChar + 1U);
        PRINTF("Press %c to generate triangle waveform0.\r\n", cSawtoothWaveform0InputChar + 2U);
        PRINTF("Press %c to generate triangle waveform1.\r\n", cSawtoothWaveform0InputChar + 3U);

        PRINTF("\r\nWaiting for the waveform select...\r\n");

        /* Waits for user response. */
        u8Ch = GETCHAR();

        if ((u8Ch >= 'a') && (u8Ch <= 'z'))
        {
            u8Ch -= 'a' - 'A';
        }

        if ((u8Ch >= cSawtoothWaveform0InputChar) && (u8Ch <= cSawtoothWaveform0InputChar + 3U))
        {
            eWaveformType = (dac_waveform_type_t)(u8Ch - cSawtoothWaveform0InputChar);
            DAC_EnableAnalogPortion(DEMO_DAC_BASE, false);
            APP_GetWaveformAttribute(&sWaveformAttribute);

            eStatus = APP_SetWaveformAttribute(&sWaveformAttribute, eWaveformType, DEMO_CLOCK_FREQUENCY);
            if (eStatus == kStatus_Success)
            {
                PRINTF("\r\nSucceeded to generate the desired waveform.\r\n");
            }
            else
            {
                PRINTF("\r\nFailed to generate the desired waveform.\r\n");
            }
            PRINTF("\r\nPress any key to generate another waveform.\r\n");
        }
        else
        {
            PRINTF("\r\n Wrong Input Key, please press any key to continue.\r\n");
        }
        u8Ch = GETCHAR();
    }
}

/*!
 * @brief Gets the waveform attributes from the user inputs.
 */
static void APP_GetWaveformAttribute(dac_waveform_attribute_t *psWaveformAttr)
{
    uint32_t u32Value;

    while (1)
    {
        PRINTF("\r\nPlease input minimum value of waveform:");
        u32Value = APP_GetInputValue();
        if (u32Value > 0xFFFU)
        {
            PRINTF("\r\nYour value is out of range.\r\n");
            continue;
        }
        psWaveformAttr->u16MinValue = u32Value;

        PRINTF("\r\nPlease input maximum value of waveform:");
        u32Value = APP_GetInputValue();
        if (u32Value > 0xFFFU)
        {
            PRINTF("\r\nYour value is out of range.\r\n");
            continue;
        }
        psWaveformAttr->u16MaxValue = u32Value;

        PRINTF("\r\nPlease input step size of waveform:");
        u32Value = APP_GetInputValue();
        if (u32Value > 0xFFFU)
        {
            PRINTF("\r\nYour value is out of range.\r\n");
            continue;
        }

        psWaveformAttr->u16StepSize = u32Value;

        PRINTF("\r\nPlease input start value of waveform:");
        u32Value = APP_GetInputValue();
        if ((u32Value > psWaveformAttr->u16MaxValue) || (u32Value < psWaveformAttr->u16MinValue))
        {
            PRINTF("\r\n The start value is out of range.\r\n");
            continue;
        }
        psWaveformAttr->u16StartValue = u32Value;

        PRINTF("\r\nPlease input the desired frequency of the waveform:");
        u32Value                        = APP_GetInputValue();
        psWaveformAttr->u32WaveformFreq = u32Value;
        break;
    }
}

/*!
 * @brief Sets the waveform attributes.
 */
static status_t APP_SetWaveformAttribute(const dac_waveform_attribute_t *psWaveformAttr,
                                         dac_waveform_type_t eWaveformType,
                                         uint32_t u32ClockSourceFreq)
{
    status_t eStatus;

    if (psWaveformAttr->u16MinValue < psWaveformAttr->u16MaxValue)
    {
        switch (eWaveformType)
        {
            case kDAC_RepeatSawtoothWaveform0:
                if (psWaveformAttr->u16StepSize < (psWaveformAttr->u16MaxValue - psWaveformAttr->u16MinValue))
                {
                    eStatus = APP_GenerateSawtoothWaveform(psWaveformAttr, u32ClockSourceFreq, true);
                }
                else
                {
                    eStatus = kStatus_Fail;
                }
                break;
            case kDAC_RepeatSawtoothWaveform1:
                if (psWaveformAttr->u16StepSize < (psWaveformAttr->u16MaxValue - psWaveformAttr->u16MinValue))
                {
                    eStatus = APP_GenerateSawtoothWaveform(psWaveformAttr, u32ClockSourceFreq, false);
                }
                else
                {
                    eStatus = kStatus_Fail;
                }
                break;
            case kDAC_RepeatTriangleWaveform0:
                if (psWaveformAttr->u16StepSize < (psWaveformAttr->u16MaxValue - psWaveformAttr->u16MinValue))
                {
                    eStatus = APP_GenerateTriangleWaveform(psWaveformAttr, u32ClockSourceFreq, true);
                }
                else
                {
                    eStatus = kStatus_Fail;
                }
                break;
            case kDAC_RepeatTriangleWaveform1:
                if (psWaveformAttr->u16StepSize < (psWaveformAttr->u16MaxValue - psWaveformAttr->u16MinValue))
                {
                    eStatus = APP_GenerateTriangleWaveform(psWaveformAttr, u32ClockSourceFreq, false);
                }
                else
                {
                    eStatus = kStatus_Fail;
                }
                break;
            default:
                eStatus = kStatus_Fail;
                break;
        }
    }
    else
    {
        eStatus = kStatus_Fail;
    }

    return eStatus;
}

/*!
 * @brief Generates sawtooth waveform based on the attributes.
 */
static status_t APP_GenerateSawtoothWaveform(const dac_waveform_attribute_t *psWaveformAttr,
                                             uint32_t u32ClockSourceFreq,
                                             bool bIncrease)
{
    uint16_t u16DividerValue = 0U;
    uint16_t u16StepSize;
    double dTmpDivider       = 0.0f;
    double dLimitu16StepSize = 0.0f;
    dac_config_t sSawtoothWaveformConfig;

    /*
        Due to DAC's settling time, to generate the Sawtooth Waveform with specific frequency and
        specific amplitude, the step size should satisfy the following relationship.
            u16StepSize >= Maximum Settling time * (maxValue - minValue) * waveformFreq
    */
    dLimitu16StepSize = DEMO_DAC_MAX_SETTING_TIME * (psWaveformAttr->u32WaveformFreq) *
                        (psWaveformAttr->u16MaxValue - psWaveformAttr->u16MinValue);
    dLimitu16StepSize /= 1000000.0f;
    if (psWaveformAttr->u16StepSize >= dLimitu16StepSize)
    {
        u16StepSize = psWaveformAttr->u16StepSize;
    }
    else
    {
        if (psWaveformAttr->bAutoAdjustStep)
        {
            /* If enabled bAutoAdjustStep feature, the driver will auto adjust the step size. */
            u16StepSize = (uint16_t)(dLimitu16StepSize + 1U);

            /* The u16StepSize is also limited by the width of register. */
            if (u16StepSize > 0xFFFU)
            {
                return kStatus_Fail;
            }
        }
        else
        {
            return kStatus_Fail;
        }
    }

    /* Calculates compare value based on DAC clock frequency and desired output waveform frequency. */
    dTmpDivider = (u32ClockSourceFreq / (psWaveformAttr->u32WaveformFreq)) * u16StepSize;
    dTmpDivider /= (double)(psWaveformAttr->u16MaxValue - psWaveformAttr->u16MinValue);
    if (dTmpDivider > 65536.0f)
    {
        /* The divider value is limited by the width of the COMPARE register. */
        return kStatus_Fail;
    }
    u16DividerValue = (uint16_t)(dTmpDivider - 1.0f);

    DAC_GetDefaultConfig(&sSawtoothWaveformConfig);
    sSawtoothWaveformConfig.bEnableAnalogPortion = true;
    sSawtoothWaveformConfig.eOperationMode       = kDAC_AutomaticOperationMode;
    if (bIncrease)
    {
        sSawtoothWaveformConfig.uOperationConfig.sAutomaticModeConfig.eWaveformType = kDAC_RepeatSawtoothWaveform0;
    }
    else
    {
        sSawtoothWaveformConfig.uOperationConfig.sAutomaticModeConfig.eWaveformType = kDAC_RepeatSawtoothWaveform1;
    }
    sSawtoothWaveformConfig.uOperationConfig.sAutomaticModeConfig.u16CompareValue = u16DividerValue;
    sSawtoothWaveformConfig.uOperationConfig.sAutomaticModeConfig.u16MaxValue     = psWaveformAttr->u16MaxValue;
    sSawtoothWaveformConfig.uOperationConfig.sAutomaticModeConfig.u16MinValue     = psWaveformAttr->u16MinValue;
    sSawtoothWaveformConfig.uOperationConfig.sAutomaticModeConfig.u16StartValue   = psWaveformAttr->u16StartValue;
    sSawtoothWaveformConfig.uOperationConfig.sAutomaticModeConfig.u16StepSize     = u16StepSize;
    DAC_Init(DEMO_DAC_BASE, &sSawtoothWaveformConfig);

    return kStatus_Success;
}

/*!
 * @brief Generates triangle waveform based on the attributes.
 */
static status_t APP_GenerateTriangleWaveform(const dac_waveform_attribute_t *psWaveformAttr,
                                             uint32_t u32ClockSourceFreq,
                                             bool bIncrease)
{
    uint16_t u16DividerValue = 0U;
    uint16_t u16StepSize;
    double dTmpDivider    = 0.0f;
    double dLimitStepSize = 0.0f;
    dac_config_t sTriangleWaveformConfig;

    /*
        Due to DAC's settling time, to generate the Triangle Waveform with specific frequency and
        specific amplitude, the step size should satisfy the following relationship.
            u16StepSize >= 2 * Settling time * (u16MaxValue - u16MinValue) * u32WaveformFreq
    */
    dLimitStepSize = 2.0f * DEMO_DAC_MAX_SETTING_TIME * (psWaveformAttr->u32WaveformFreq) *
                     (psWaveformAttr->u16MaxValue - psWaveformAttr->u16MinValue);
    dLimitStepSize /= 1000000.0f;
    if (psWaveformAttr->u16StepSize >= dLimitStepSize)
    {
        u16StepSize = psWaveformAttr->u16StepSize;
    }
    else
    {
        if (psWaveformAttr->bAutoAdjustStep)
        {
            /* If enabled autoAdjustStep feature, the driver will auto adjust the step size. */
            u16StepSize = (uint16_t)(dLimitStepSize + 1U);
            if (u16StepSize > 0xFFFU)
            {
                /* The u16StepSize is also limited by the width of register. */
                return kStatus_Fail;
            }
        }
        else
        {
            return kStatus_Fail;
        }
    }

    /* Calculates compare value based on DAC clock frequency and desired output waveform frequency. */
    dTmpDivider = (u32ClockSourceFreq / (psWaveformAttr->u32WaveformFreq) * (u16StepSize));
    dTmpDivider /= 2.0f * (double)(psWaveformAttr->u16MaxValue - psWaveformAttr->u16MinValue);
    if (dTmpDivider > 65536.0f)
    {
        /* The divider value is limited by the width of the COMPARE register. */
        return kStatus_Fail;
    }
    u16DividerValue = (uint16_t)(dTmpDivider - 1.0f);

    DAC_GetDefaultConfig(&sTriangleWaveformConfig);
    sTriangleWaveformConfig.bEnableAnalogPortion = true;
    sTriangleWaveformConfig.eOperationMode       = kDAC_AutomaticOperationMode;
    if (bIncrease)
    {
        sTriangleWaveformConfig.uOperationConfig.sAutomaticModeConfig.eWaveformType = kDAC_RepeatTriangleWaveform0;
    }
    else
    {
        sTriangleWaveformConfig.uOperationConfig.sAutomaticModeConfig.eWaveformType = kDAC_RepeatTriangleWaveform1;
    }
    sTriangleWaveformConfig.uOperationConfig.sAutomaticModeConfig.u16CompareValue = u16DividerValue;
    sTriangleWaveformConfig.uOperationConfig.sAutomaticModeConfig.u16MaxValue     = psWaveformAttr->u16MaxValue;
    sTriangleWaveformConfig.uOperationConfig.sAutomaticModeConfig.u16MinValue     = psWaveformAttr->u16MinValue;
    sTriangleWaveformConfig.uOperationConfig.sAutomaticModeConfig.u16StartValue   = psWaveformAttr->u16StartValue;
    sTriangleWaveformConfig.uOperationConfig.sAutomaticModeConfig.u16StepSize     = u16StepSize;
    DAC_Init(DEMO_DAC_BASE, &sTriangleWaveformConfig);

    return kStatus_Success;
}

/*!
 * @brief The function to get input values.
 */
static uint32_t APP_GetInputValue(void)
{
    uint8_t ch          = 0;
    uint32_t inputValue = 0UL;
    while (ch != 0x0D)
    {
        ch = GETCHAR();
        if ((ch >= '0') && (ch <= '9'))
        {
            PUTCHAR(ch);
            inputValue = inputValue * 10 + (ch - 0x30U);
        }
    }
    return inputValue;
}
