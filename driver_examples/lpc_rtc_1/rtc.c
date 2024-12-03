/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_rtc.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool sleepEnd;
rtc_datetime_t date;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief ISR for sleep interrupt
 *
 * This function changes the state of sleepEnd.
 */
void RTC_IRQHandler(void)
{
    if (kRTC_CountDownMaskedStatus == RTC_GetMaskedInterruptStatus(RTC))
    {
        sleepEnd = true;
        RTC_ClearMaskedInterruptStatus(RTC, kRTC_CountDownMaskedStatus);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t sleepTime = 0U;
    rtc_config_t userConfig;

    BOARD_InitHardware();

    RTC_GetDefaultConfig(RTC, &userConfig);
    /* Set a start data time. */
    userConfig.date.year   = 2018U;
    userConfig.date.month  = 1U;
    userConfig.date.day    = 1U;
    userConfig.date.hour   = 0U;
    userConfig.date.minute = 0U;
    userConfig.date.second = 0U;

    /* Init RTC module. */
    RTC_Init(RTC, &userConfig);

    /* Stop RTC to setup date. */
    RTC_StopTimer(RTC);

    /* Enable RTC alarm interrupt */
    RTC_EnableInterrupts(RTC, kRTC_CountDownInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(RTC_IRQn);

    /* Start the RTC time counter */
    RTC_StartTimer(RTC);

    while (1)
    {
        sleepEnd  = false;
        sleepTime = 1U;

        RTC_SetSleepTime(RTC, sleepTime);

        /* Wait for sleep end. */
        while (!sleepEnd)
        {
        }
        /* Get the format date. */
        RTC_GetDatetime(RTC, &date);

        /* Toggle the LED. */
        GPIO_PortToggle(GPIO, 1U << EXAMPLE_LED_CONTROL_PIN);
    }
}
