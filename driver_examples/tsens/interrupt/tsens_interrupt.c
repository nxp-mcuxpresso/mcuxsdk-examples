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
volatile bool isHigh;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief TSENS ISR.
 */
void TSEN_IRQHandler(void)
{
    uint32_t status          = TSENS_GetStatusFlags(DEMO_TSENS);
    uint32_t interruptStatus = TSENS_GetInterruptStatusFlags(DEMO_TSENS);
    TSENS_ClearInterruptStatusFlags(DEMO_TSENS, kTSENS_HighThresholdInterruptFlag | kTSENS_LowThresholdInterruptFlag);

    if (kTSENS_HighThresholdInterruptFlag == (kTSENS_HighThresholdInterruptFlag & status))
    {
        if (kTSENS_LowThresholdInterruptEnable != (kTSENS_LowThresholdInterruptEnable & interruptStatus))
        {
            /* Enable low temperature threshold interrupt. */
            TSENS_EnableInterrupt(DEMO_TSENS, kTSENS_LowThresholdInterruptEnable);
        }
        isHigh = true;
    }
    else if (kTSENS_LowThresholdInterruptFlag == (kTSENS_LowThresholdInterruptFlag & status))
    {
        /* Disable low temperature threshold interrupt. */
        TSENS_DisableInterrupt(DEMO_TSENS, kTSENS_LowThresholdInterruptEnable);
        isHigh = false;
    }
}

/*!
 * @brief Main function.
 */
int main(void)
{
    tsens_config_t config;
    bool highThresholdLedFlag = false;

    BOARD_InitHardware();

    /*
     * config.outputMode = kTSENS_OutputCalibratedData;
     * config.resolutionMode = kTSENS_Resolution12bit;
     * config.enableVolatgeCalibration = false;
     */
    TSENS_GetDefaultConfig(&config);
    TSENS_Init(DEMO_TSENS, &config);

    /* Set the high temperature threshold 30 degrees Celsius. */
    TSENS_SetHighTemperatureThreshold(DEMO_TSENS, g_MonitorHighThreshold);
    /* Set the low temperature threshold 28 degrees Celsius. */
    TSENS_SetLowTemperatureThreshold(DEMO_TSENS, g_MonitorLowThreshold);

    /* Enable high temperature threshold interrupt. */
    TSENS_EnableInterrupt(DEMO_TSENS, kTSENS_HighThresholdInterruptEnable);
    /* Enable tsens interrupt at the NVIC */
    EnableIRQ(TSEN_IRQn);

    while (1)
    {
        TSENS_StartMeasurement(DEMO_TSENS);

        if (isHigh && !highThresholdLedFlag)
        {
            /* Toggle the LED. */
            GPIO_PortToggle(DEMO_LED_GPIO_BASE, 1U << DEMO_LED_CONTROL_PIN);
            highThresholdLedFlag = true;
        }
        else if (!isHigh && highThresholdLedFlag)
        {
            /* Toggle the LED. */
            GPIO_PortToggle(DEMO_LED_GPIO_BASE, 1U << DEMO_LED_CONTROL_PIN);
            highThresholdLedFlag = false;
        }
    }
}
