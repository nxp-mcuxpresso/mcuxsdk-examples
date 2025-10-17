/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_bctu.h"
#include "fsl_sar_adc.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DEMO_BCTU_IRQ_HANDLER_FUNC(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool wmOverflow = false;
static bctu_fifo_res_t result[3U];

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_BCTU_IRQ_HANDLER_FUNC(void)
{
    if (DEMO_BCTU_INT_MASK == (BCTU_GetFifoStatusFlags(DEMO_BCTU_BASE) & DEMO_BCTU_INT_MASK))
    {
        for(uint8_t index = 0U; index < 3U; ++index)
        {
            BCTU_GetFifoResult(DEMO_BCTU_BASE, DEMO_BCTU_FIFO_INDEX, &(result[index]));
        }

        BCTU_ClearFifoStatusFlags(DEMO_BCTU_BASE, DEMO_BCTU_INT_MASK);
        wmOverflow = true;
    }
}

static void DEMO_AdcConfig(void)
{
    adc_config_t adcConfig;
    adc_calibration_config_t calibrationConfig;

    ADC_GetDefaultConfig(&adcConfig);
    adcConfig.clockFrequency    = DEMO_ADC_CLOCK_FREQ;
    adcConfig.bctuMode          = DEMO_ADC_BCTU_MODE;
    adcConfig.convAvg           = DEMO_ADC_CONV_AVERAGE;
    ADC_Init(DEMO_ADC_BASE_0, &adcConfig);
    ADC_Init(DEMO_ADC_BASE_1, &adcConfig);
    ADC_Init(DEMO_ADC_BASE_2, &adcConfig);

    calibrationConfig.enableAverage        = DEMO_ADC_CAL_AVERAGE_ENABLE;
    calibrationConfig.sampleTime           = DEMO_ADC_CAL_SAMPLE_TIME;
    calibrationConfig.averageSampleNumbers = DEMO_ADC_CAL_AVERAGE_SAMPLE_NUM;

    if (!(ADC_DoCalibration(DEMO_ADC_BASE_0, &calibrationConfig)))
    {
        (void)PRINTF("ADC 0 calibration failed\r\n");
    }
    if (!(ADC_DoCalibration(DEMO_ADC_BASE_1, &calibrationConfig)))
    {
        (void)PRINTF("ADC 1 calibration failed\r\n");
    }
    if (!(ADC_DoCalibration(DEMO_ADC_BASE_2, &calibrationConfig)))
    {
        (void)PRINTF("ADC 2 calibration failed\r\n");
    }
}

static void DEMO_BctuConfig(void)
{
    bctu_config_t config;
    bctu_trig_config_t trigConfig;

    bctu_convlist_config_t convListConfig[2U] = {
      {
            .lastChan           = false,
            .lastChanPlusOne    = false,
            .waitTrig           = false,
            .waitTrigPlusOne    = false,
            .adcChan            = DEMO_BCTU_ADC_CHANNEL_0,
            .adcChanPlusOne     = DEMO_BCTU_ADC_CHANNEL_1,
      },
      {
            .lastChan           = true,
            .lastChanPlusOne    = false,
            .waitTrig           = false,
            .waitTrigPlusOne    = false,
            .adcChan            = DEMO_BCTU_ADC_CHANNEL_2,
      },
    };

    BCTU_GetDefaultConfig(&config);
    config.writeProtect = DEMO_BCTU_REG_PROTECT;
    BCTU_Init(DEMO_BCTU_BASE, &config);

    trigConfig.trigIndex = DEMO_BCTU_TRIGGER_INDEX;
    trigConfig.chanAddr = DEMO_BCTU_TRIG_CHAN_ADDR;
    trigConfig.dataDest = DEMO_BCTU_DATA_DEST;
    trigConfig.enableLoop = DEMO_BCTU_LOOP_ENABLE;
    trigConfig.targetAdc = DEMO_BCTU_TRIG_ADC_INSTANCE;
    trigConfig.trigRes = DEMO_BCTU_TRIG_RESOLUTION;
    BCTU_SetTrigConfig(DEMO_BCTU_BASE, &trigConfig);

    for(uint8_t index = 0U; index < 2U; ++index)
    {
        BCTU_SetConvListConfig(DEMO_BCTU_BASE, &(convListConfig[index]), index);
    }

    BCTU_SetFifoWaterMark(DEMO_BCTU_BASE, DEMO_BCTU_FIFO_INDEX, (3U - 1U));
    BCTU_EnableFifoInt(DEMO_BCTU_BASE, DEMO_BCTU_INT_MASK, true);
    (void)EnableIRQ(DEMO_BCTU_IRQn);
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    DEMO_AdcConfig();
    DEMO_BctuConfig();

    BCTU_EnableModule(DEMO_BCTU_BASE, true);
    BCTU_EnableGlobalTrig(DEMO_BCTU_BASE, true);

    (void)PRINTF("BCTU SoftWare Trigger Multiple Parallel Conversions Example.\r\n");
    (void)PRINTF("Please press any key to get the ADC value\r\n");

    while (true)
    {
        (void)GETCHAR();

        BCTU_EnableSoftwareTrig(DEMO_BCTU_BASE, DEMO_BCTU_SW_TRIG_GROUP, (uint32_t)DEMO_BCTU_SW_TRIG_MASK);

        while (!wmOverflow)
        {
        }

        for(uint8_t index = 0U; index < 3U; ++index)
        {
            (void)PRINTF("ADC_%d channel %d value: %d\r\n", result[index].adcNum, result[index].chanNum, result[index].convRes);
        }

        wmOverflow = false;
    }
}
