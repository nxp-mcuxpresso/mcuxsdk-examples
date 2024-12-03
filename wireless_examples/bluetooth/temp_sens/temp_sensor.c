/*! *********************************************************************************
* \addtogroup Temperature Sensor
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright 2015 Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2023-2024 NXP
*
*
* \file
*
* This file is the source file for the Temperature Sensor application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "board.h"
#include "fsl_adc16.h"
/************************************************************************************
*************************************************************************************
* Private type definitions and macros
*************************************************************************************
************************************************************************************/
/*!
 * @brief These values are used to get the temperature. DO NOT MODIFY
 * The method used in this demo to calculate temperature of chip is mapped to
 * Temperature Sensor for the HCS08 Microcontroller Family document (Document Number: AN3031)
 */
#define ADCR_VDD                (65535U)    /* Maximum value when use 16b resolution */
#define V_BG                    (1000U)     /* BANDGAP voltage in mV (trim to 1.0V) */
#define V_TEMP25                (716U)      /* Typical converted value at 25 oC in mV */
#define M                       (1620U)     /* Typical slope:uV/oC */
#define STANDARD_TEMP           (25)

#define ADC16_INSTANCE                (0)           /* ADC instance */
#define ADC16_TEMPERATURE_CHN         (kAdc16Chn26) /* Temperature Sensor Channel */
#define ADC16_BANDGAP_CHN             (kAdc16Chn27) /* ADC channel of BANDGAP */
#define ADC16_CHN_GROUP               (0)           /* ADC group configuration selection */

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

uint32_t adcValue = 0;               /* ADC value */
uint32_t adcrTemp25 = 0;             /* Calibrated ADCR_TEMP25 */
uint32_t adcr100m = 0;               /* calibrated conversion value of 100mV */
/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void TempSensor_Init(void)
{
    /* Use 16bit resolution if enable. */
#if (FSL_FEATURE_ADC16_MAX_RESOLUTION >= 16)
    adcUserConfig.resolution = kAdc16ResolutionBitOf16;
#endif


}


int32_t TempSensor_GetTemperature(void)
{
    int32_t currentTemp = 0;
    adc16_chn_config_t chnConfig;

    /* Configure the conversion channel
     * differential and interrupt mode disable. */
    chnConfig.chnIdx     = (adc16_chn_t)ADC16_TEMPERATURE_CHN;
#if FSL_FEATURE_ADC16_HAS_DIFF_MODE
    chnConfig.diffConvEnable = false;
#endif
    chnConfig.convCompletedIntEnable  = false;

    /* Software trigger the conversion. */
    ADC16_ConfigConvChn(ADC16_INSTANCE, ADC16_CHN_GROUP, &chnConfig);

    /* Wait for the conversion to be done. */
    ADC16_WaitConvDone(ADC16_INSTANCE, ADC16_CHN_GROUP);

    /* Fetch the conversion value. */
    adcValue = ADC16_GetConvValueSigned(ADC16_INSTANCE, ADC16_CHN_GROUP);

    /* Calculates adcValue in 16bit resolution
     * from 12bit resolution in order to convert to temperature. */
#if (FSL_FEATURE_ADC16_MAX_RESOLUTION < 16)
    adcValue = adcValue << 4;
#endif

    /* Multiplied by 1000 because M in uM/oC
     * Temperature = 25 - (ADCR_T - ADCR_TEMP25) * 100*1000 / ADCR_100M*M */
    currentTemp = (int32_t)(STANDARD_TEMP - ((int32_t)adcValue - (int32_t)adcrTemp25) * 100000 /(int32_t)(adcr100m*M));

    /* Pause the conversion. */
    ADC16_PauseConv(ADC16_INSTANCE, ADC16_CHN_GROUP);

    return currentTemp;
}


/*! *********************************************************************************
* @}
********************************************************************************** */
