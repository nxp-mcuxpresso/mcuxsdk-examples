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

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("\n\rSFA example -- Measure the frequency of clock under test in polling mode\r\n");
    SFA_Init(DEMO_SFA_BASEADDR);

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
    uint32_t freq;
    status_t status;
    sfa_config_t config;

    CLOCK_SetClkOutSel(kClockClkoutSelSosc);
    SFA_GetDefaultConfig(&config);
    APP_SetupFastClock(&config);

    SFA_SetMeasureConfig(DEMO_SFA_BASEADDR, &config);
    PRINTF("Start to measure Fast Clock.\r\n");
    status = SFA_MeasureBlocking(DEMO_SFA_BASEADDR);

    if (status == kStatus_SFA_MeasurementCompleted)
    {
        freq = SFA_CalculateFrequencyOrPeriod(DEMO_SFA_BASEADDR, DEMO_SFA_REF_FREQ);
        PRINTF("The actual frequency of Fast Clock is %d Hz.\n\r", freq);
    }
    else if (status == kStatus_SFA_ReferenceCounterTimeout)
    {
        PRINTF("\r\nSFA reference counter timeout happens\r\n");
    }
}

/*!
 * @brief Measure the frequency of the clock that slower than the reference clock
 */
static void APP_MeasureSlowClockFreq(void)
{
    uint32_t freq;
    status_t status;
    sfa_config_t config;

    SFA_GetDefaultConfig(&config);
    APP_SetupSlowClock(&config);

    PRINTF("Start to measure Slow clock.\r\n");
    SFA_SetMeasureConfig(DEMO_SFA_BASEADDR, &config);
    status = SFA_MeasureBlocking(DEMO_SFA_BASEADDR);

    if (status == kStatus_SFA_MeasurementCompleted)
    {
        freq = SFA_CalculateFrequencyOrPeriod(DEMO_SFA_BASEADDR, DEMO_SFA_REF_FREQ);
        PRINTF("The actual frequency of the Slow Clock is %d Hz.\r\n", freq);
    }
    else if (status == kStatus_SFA_ReferenceCounterTimeout)
    {
        PRINTF("\r\nSFA reference counter timeout happens\r\n");
    }
}
