/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_sfa.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_MeasureFastClockFreq(void);
static void APP_MeasureSlowClockFreq(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool SfaMeasureFinished = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief SFA callback function
 */
void EXAMPLE_SFA_CALLBACK(status_t status)
{
    if (status == kStatus_SFA_MeasurementCompleted)
    {
        SfaMeasureFinished = true;
    }
    else if (status == kStatus_SFA_ReferenceCounterTimeout)
    {
        PRINTF("\r\nSFA reference counter timeout happens\r\n");
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("\r\nSFA Example -- Measure the frequency of clock under test in interrupt mode.\r\n");

    /* Enable SFA interrupt. */
    EnableIRQ(SFA_IRQn);

    /* Set SFA interrupt priority. */
    NVIC_SetPriority(SFA_IRQn, 1);

    SFA_Init(DEMO_SFA_BASEADDR);
    SFA_InstallCallback(DEMO_SFA_BASEADDR, EXAMPLE_SFA_CALLBACK);

    /* Measure Faster clock frequency. */
    APP_MeasureFastClockFreq();

    /* Measure Slower clock frequency. */
    APP_MeasureSlowClockFreq();

    PRINTF("\r\nSFA Measurement Finished.\r\n");
    while (1)
    {
    }
}

/*!
 * @brief Measure the frequency of the clock that faster than the reference clock.
 */
static void APP_MeasureFastClockFreq(void)
{
    uint32_t freq = 0UL;
    sfa_config_t config;

    SFA_GetDefaultConfig(&config);
    APP_SetupFastClock(&config);

    SFA_SetMeasureConfig(DEMO_SFA_BASEADDR, &config);
    PRINTF("Start to measure Fast Clock.\r\n");
    SFA_MeasureNonBlocking(DEMO_SFA_BASEADDR);

    while (1)
    {
        if (SfaMeasureFinished)
        {
            SfaMeasureFinished = false;
            freq               = SFA_CalculateFrequencyOrPeriod(DEMO_SFA_BASEADDR, DEMO_SFA_REF_FREQ);
            PRINTF("The actual frequency of Fast Clock is %d Hz.\n\r", freq);
            break;
        }
        else
        {
            __WFI();
        }
    }
}

/*!
 * @brief Measure the frequency of the clock that slower than the reference clock
 */
static void APP_MeasureSlowClockFreq(void)
{
    uint32_t freq = 0UL;
    sfa_config_t config;

    SFA_GetDefaultConfig(&config);
    APP_SetupSlowClock(&config);

    SFA_SetMeasureConfig(DEMO_SFA_BASEADDR, &config);
    PRINTF("Start to measure Slow clock.\r\n");
    SFA_MeasureNonBlocking(DEMO_SFA_BASEADDR);

    while (1)
    {
        if (SfaMeasureFinished)
        {
            SfaMeasureFinished = false;
            freq               = SFA_CalculateFrequencyOrPeriod(DEMO_SFA_BASEADDR, DEMO_SFA_REF_FREQ);
            PRINTF("The actual frequency of the Slow Clock is %d Hz.\n\r", freq);
            break;
        }
        else
        {
            __WFI();
        }
    }
}
