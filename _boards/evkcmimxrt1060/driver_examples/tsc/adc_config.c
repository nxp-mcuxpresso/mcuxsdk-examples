/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_adc.h"
#include "app.h"
#include "fsl_debug_console.h"

void ADC_Configuration(void)
{
    adc_config_t k_adcConfig;
    adc_channel_config_t k_adcChannelConfig;

    /* Initialize the ADC module. */
    ADC_GetDefaultConfig(&k_adcConfig);
    ADC_Init(DEMO_ADC_BASE, &k_adcConfig);
    ADC_EnableHardwareTrigger(DEMO_ADC_BASE, true);

    /* Before TSC starts work, software driver configure ADC_HCx.
     * For four-wire resistive screen, x-coordinate measure triggers to ADC_HC3
     * and y-coordinate measure triggers to ADC_HC1. So we need configure ADC_HC1
     * and ADC_HC3 to make sure TSC could work normally.
     */
    k_adcChannelConfig.channelNumber                        = 1U; /* Channel1 is ynlr port. */
    k_adcChannelConfig.enableInterruptOnConversionCompleted = false;
    ADC_SetChannelConfig(DEMO_ADC_BASE, 3U, &k_adcChannelConfig);
    k_adcChannelConfig.channelNumber = 3U; /* Channel3 is xnur port. */
    ADC_SetChannelConfig(DEMO_ADC_BASE, 1U, &k_adcChannelConfig);

    /* Do auto hardware calibration. */
    if (kStatus_Success == ADC_DoAutoCalibration(DEMO_ADC_BASE))
    {
        PRINTF("ADC_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC_DoAutoCalibration() Failed.\r\n");
    }
}
