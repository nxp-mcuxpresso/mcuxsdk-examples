/*
 * Copyright  2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_lpadc.h"
#include "fsl_clock.h"
#include "board_utility.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bool adcInitialized                                  = false;
static BOARD_AdcTriggerState triggerState                   = BOARD_AdcTriggerNone;
static lpadc_reference_voltage_source_t currentADCReference = kLPADC_ReferenceVoltageAlt1;
static lpadc_calibration_value_t adcCalibrationValue;

#if (defined(LPADC_USE_HIGH_RESOLUTION) && LPADC_USE_HIGH_RESOLUTION)
const uint32_t g_LpadcFullRange   = 65536U;
const uint32_t g_LpadcResultShift = 0U;
#else
const uint32_t g_LpadcFullRange   = 4096U;
const uint32_t g_LpadcResultShift = 3U;
#endif /* DEMO_LPADC_USE_HIGH_RESOLUTION */
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * brief Init the ADC with the specific ADC reference.
 *
 * param reference The specific ADC reference.
 */
static void initAdcReference(lpadc_reference_voltage_source_t reference)
{
    lpadc_config_t mLpadcConfigStruct;

    LPADC_GetDefaultConfig(&mLpadcConfigStruct);
    mLpadcConfigStruct.enableAnalogPreliminary = true;
    mLpadcConfigStruct.referenceVoltageSource  = reference;
    mLpadcConfigStruct.conversionAverageMode   = kLPADC_ConversionAverage128;
    mLpadcConfigStruct.FIFOWatermark           = FSL_FEATURE_LPADC_TEMP_SENS_BUFFER_SIZE - 1U;
    LPADC_Init(LPADC_BASE, &mLpadcConfigStruct);

    currentADCReference = reference;
}

/*!
 * brief Basic Init of ADC module
 */
static void initAdcBasic(void)
{
    CLOCK_EnableClock(kCLOCK_Lpadc0);
    CLOCK_SetIpSrc(kCLOCK_Lpadc0, kCLOCK_IpSrcFro192M);
    CLOCK_SetIpSrcDiv(kCLOCK_Lpadc0, kSCG_SysClkDivBy4);

    initAdcReference(LPADC_REFERENCE_BATTERY);
}

/*!
 * brief Check if ADC driver is initialized.
 */
bool BOARD_IsAdcInitialized(void)
{
    return adcInitialized;
}

/*!
 * brief Init ADC driver.
 */
void BOARD_InitAdc(void)
{
    if (false == BOARD_IsAdcInitialized())
    {
        initAdcBasic();

        /* Request offset calibration. Takes around 140us */
        LPADC_DoOffsetCalibration(LPADC_BASE);
        /* Request gain calibration and wait for it to be done, takes around 3.5ms */
        LPADC_DoAutoCalibration(LPADC_BASE);
    }

    adcInitialized = true;
}

/*!
 * brief Reset ADC initialized flag and de-initializes the ADC module.
 */
void BOARD_DeinitAdc(void)
{
    if (true == BOARD_IsAdcInitialized())
    {
        adcInitialized = false;
        /* Save ADC calibration data to be restored by reinit function */
        LPADC_GetCalibrationValue(LPADC_BASE, &adcCalibrationValue);
        LPADC_Deinit(LPADC_BASE);
    }
}

/*!
 * brief Reinit ADC driver.
 */
void BOARD_ReinitAdc(void)
{
    if (false == BOARD_IsAdcInitialized())
    {
        initAdcBasic();

        /* Request offset calibration */
        LPADC_DoOffsetCalibration(LPADC_BASE);
        /* Set calibration data saved before deinit */
        /* Offset calibration allows to set ADC_STAT_CAL_RDY bit which is required
         * by LPADC_SetCalibrationValue.
         */
        LPADC_SetCalibrationValue(LPADC_BASE, &adcCalibrationValue);
    }

    adcInitialized = true;
}

/*!
 * brief Do software trigger to conversion command.
 */
void BOARD_AdcSwTrigger(uint32_t channel)
{
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;

    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);

    mLpadcCommandConfigStruct.channelNumber = channel;
    if (LPADC_TEMPERATURE_SENSOR_CHANNEL == channel)
    {
        if (currentADCReference != LPADC_REFERENCE_TEMPERATURE)
        {
            initAdcReference(LPADC_REFERENCE_TEMPERATURE);
        }

        mLpadcCommandConfigStruct.sampleChannelMode   = kLPADC_SampleChannelSingleEndSideA;
        mLpadcCommandConfigStruct.sampleTimeMode      = kLPADC_SampleTimeADCK35;
        mLpadcCommandConfigStruct.hardwareAverageMode = kLPADC_HardwareAverageCount16;
        mLpadcCommandConfigStruct.loopCount           = 1U;

        triggerState = BOARD_AdcTriggerTemperature;
    }
    else if (LPADC_BATTERY_MONITOR_CHANNEL == channel)
    {
        if (currentADCReference != LPADC_REFERENCE_BATTERY)
        {
            initAdcReference(LPADC_REFERENCE_BATTERY);
        }

        mLpadcCommandConfigStruct.sampleChannelMode   = kLPADC_SampleChannelSingleEndSideA;
        mLpadcCommandConfigStruct.sampleTimeMode      = kLPADC_SampleTimeADCK35;
        mLpadcCommandConfigStruct.hardwareAverageMode = kLPADC_HardwareAverageCount16;
        mLpadcCommandConfigStruct.loopCount           = 0U;

        triggerState = BOARD_AdcTriggerBattery;
    }
#if defined(LPADC_USE_HIGH_RESOLUTION) && LPADC_USE_HIGH_RESOLUTION
    mLpadcCommandConfigStruct.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
#endif
    LPADC_SetConvCommandConfig(LPADC_BASE, LPADC_USER_CMDID, &mLpadcCommandConfigStruct);

    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId       = LPADC_USER_CMDID;
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;

    LPADC_SetConvTriggerConfig(LPADC_BASE, 0U, &mLpadcTriggerConfigStruct); /* Configurate the trigger0. */

    LPADC_DoSoftwareTrigger(LPADC_BASE, 1U);                                /* 1U is trigger0 mask. */
}

/*!
 * brief Calculate the temperature of the on-chip temperature sensor.
 *
 * return The temperature in degree. ADC conversion is not ready if -273.15f is returned.
 */
float BOARD_GetTemperature(void)
{
    uint16_t Vbe1         = 0U;
    uint16_t Vbe8         = 0U;
    float parameterSlope  = LPADC_TEMP_PARAMETER_A;
    float parameterOffset = LPADC_TEMP_PARAMETER_B;
    float parameterAlpha  = LPADC_TEMP_PARAMETER_ALPHA;
    float temperature     = -273.15f; /* Absolute zero degree as the incorrect return value. */
    lpadc_conv_result_t mLpadcResultConfigStruct;
    do
    {
        if (triggerState != BOARD_AdcTriggerTemperature)
        {
            /* Ensure to not be in an infinite while loop as LPADC_GetConvResult will always return false if nothing has
             * been trigger */
            break;
        }

        while (!LPADC_GetConvResult(LPADC_BASE, &mLpadcResultConfigStruct))
            ;

        Vbe1 = (mLpadcResultConfigStruct.convValue) >> g_LpadcResultShift;

        while (!LPADC_GetConvResult(LPADC_BASE, &mLpadcResultConfigStruct))
            ;

        Vbe8 = (mLpadcResultConfigStruct.convValue) >> g_LpadcResultShift;
        /* Final temperature = A*[alpha*(Vbe8-Vbe1)/(Vbe8 + alpha*(Vbe8-Vbe1))] - B. */
        temperature = parameterSlope * (parameterAlpha * ((float)Vbe8 - (float)Vbe1) /
                                        ((float)Vbe8 + parameterAlpha * ((float)Vbe8 - (float)Vbe1))) -
                      parameterOffset;

        triggerState = BOARD_AdcTriggerNone;
    } while (false);

    return temperature;
}

/*!
 * brief Get battery level.
 *
 * return The battery level in percent.
 */
int8_t BOARD_GetBatteryLevel(void)
{
    uint16_t adcValSample    = 0U;
    uint16_t adcValBattFull  = 0U;
    uint16_t adcValBattEmpty = 0U;
    int8_t batLvl            = 0U;
    float voltageReference   = LPADC_VOLTAGE_REFERENCE;
    lpadc_conv_result_t mLpadcResultConfigStruct;

    do
    {
        if (triggerState != BOARD_AdcTriggerBattery)
        {
            /* Ensure to not be in an infinite while loop as LPADC_GetConvResult will always return false if nothing has
             * been trigger */
            break;
        }

        while (!LPADC_GetConvResult(LPADC_BASE, &mLpadcResultConfigStruct))
            ;

        adcValSample = (mLpadcResultConfigStruct.convValue) >> g_LpadcResultShift;

        /* Calculate the ADC value of the battery full voltage */
        adcValBattFull = (uint16_t)(g_LpadcFullRange / voltageReference * LPADC_BATTERY_FULL_VOLTAGE / 4U);
        /* Calculate the ADC value of the battery empty voltage */
        adcValBattEmpty = (uint16_t)(g_LpadcFullRange / voltageReference * LPADC_BATTERY_EMPTY_VOLTAGE / 4U);

        triggerState = BOARD_AdcTriggerNone;

        if (adcValSample < adcValBattEmpty)
        {
            /* Return 0 if the adc sample value is lower than the adc value of the empty battery voltage */
            break;
        }

        /* Calculate the battery level percent */
        batLvl = 100U * (adcValSample - adcValBattEmpty) / (adcValBattFull - adcValBattEmpty);

    } while (false);

    return ((batLvl <= 100U) ? batLvl : 100U);
}
