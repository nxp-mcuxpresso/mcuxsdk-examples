/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_aon_lpadc.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool convCmpltFlag = false;
adc_conv_res_t convRes;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*! @brief ADC IRQ handler */
void DEMO_ADC_IRQ_HANDLER_FUNC(void)
{
    if (ADC_GetConvRes(DEMO_ADC_BASE, &convRes, DEMO_ADC_FIFO_INDEX))
    {
        convCmpltFlag = true;
    }
}

/*! @brief ADC configuration */
static void DEMO_DoAdcConfig(void)
{
    adc_config_t adcConfig;
    adc_cmd_config_t cmdConfig;
    adc_trig_config_t trigConfig;

    /* ADC configuration. */
    ADC_GetDefaultConfig(&adcConfig);
    adcConfig.dozeModeEn      = DEMO_ADC_DOZE_MODE_EN;
    adcConfig.calConvAvg      = DEMO_ADC_CAL_CONV_AVG;
    adcConfig.calConvSampTime = DEMO_ADC_CAL_CONV_SAMP_TIME;
    adcConfig.refVoltageSrc   = DEMO_ADC_VREF_SOURCE;
    adcConfig.fifo0Watermark  = DEMO_ADC_FIFO0_WATERMARK;
    ADC_Init(DEMO_ADC_BASE, &adcConfig);

    /* ADC calibration. */
    ADC_DoOffsetCal(DEMO_ADC_BASE);

    /* ADC command configration. */
    ADC_GetDefaultCmdConfig(&cmdConfig);
    cmdConfig.chanIndex         = DEMO_ADC_CHAN_INDEX;
    cmdConfig.convAvg           = DEMO_ADC_CONV_AVG;
    cmdConfig.convSampleTime    = DEMO_ADC_CONV_SAMP_TIME;
    ADC_SetCmdConfig(DEMO_ADC_BASE, DEMO_ADC_CMD_INDEX, &cmdConfig);

    /* ADC trigger configration. */
    ADC_GetDefaultTrigConfig(&trigConfig);
    trigConfig.cmdIndex = DEMO_ADC_CMD_INDEX;
    ADC_SetTrigConfig(DEMO_ADC_BASE, DEMO_ADC_TRIG_INDEX, &trigConfig);

    /* ADC interrupts */
    ADC_EnableInt(DEMO_ADC_BASE, kADC_ResFifo0WatermarkIntEn);
    EnableIRQ(DEMO_ADC_IRQ);
    
    ADC_Enable(DEMO_ADC_BASE, true);
}

/*! @brief Main function */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\nLpadc interrupt example");

    DEMO_DoAdcConfig();

    while (1)
    {
        PRINTF("\r\nPress any key to get the ADC conversion result");
        GETCHAR();

        ADC_DoSoftwareTrig(DEMO_ADC_BASE, DEMO_ADC_TRIG_INDEX);

        while (!convCmpltFlag)
        {
        }

        PRINTF("\r\nADC conversion value = %d", convRes.convVal);

        convCmpltFlag = false;
    }
}
