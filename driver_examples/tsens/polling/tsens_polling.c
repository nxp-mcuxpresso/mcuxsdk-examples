/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "app.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_tsens.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Monitor the high temperature threshold as 30 degrees Celsius. */
const uint32_t g_MonitorHighThreshold = 0x4BC9U;
/* Monitor the low temperature threshold as 28 degrees Celsius. */
const uint32_t g_MonitorLowThreshold = 0x4B49U;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function.
 */
int main(void)
{
    tsens_config_t config;
    uint32_t temperature      = 0U;
    bool highThresholdLedFlag = false;

    BOARD_InitHardware();

    /*
     * config.outputMode = kTSENS_OutputCalibratedData;
     * config.resolutionMode = kTSENS_Resolution12bit;
     * config.enableVolatgeCalibration = false;
     */
    TSENS_GetDefaultConfig(&config);
    TSENS_Init(DEMO_TSENS, &config);

    while (1)
    {
        TSENS_StartMeasurement(DEMO_TSENS);
        while (kTSENS_ConversionSuccessFlag != (kTSENS_ConversionSuccessFlag & TSENS_GetStatusFlags(DEMO_TSENS)))
        {
        }
        temperature = TSENS_GetTemperature(DEMO_TSENS);

        if ((temperature > g_MonitorHighThreshold) && !highThresholdLedFlag)
        {
            /* Toggle the LED. */
            GPIO_PortToggle(GPIO, 1U << DEMO_LED_CONTROL_PIN);
            highThresholdLedFlag = true;
        }
        else if ((temperature < g_MonitorLowThreshold) && highThresholdLedFlag)
        {
            /* Toggle the LED. */
            GPIO_PortToggle(GPIO, 1U << DEMO_LED_CONTROL_PIN);
            highThresholdLedFlag = false;
        }
    }
}
