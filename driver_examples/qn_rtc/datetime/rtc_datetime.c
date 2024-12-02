/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_rtc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t g_RtcSecondFlag;
rtc_datetime_t g_RtcTime;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware. */
    BOARD_InitHardware();
    PRINTF("\r\nRTC second example.\r\n");

    RTC_Init(RTC);

    /* Set a start date time and start RT */
    g_RtcTime.year   = 2014U;
    g_RtcTime.month  = 12U;
    g_RtcTime.day    = 25U;
    g_RtcTime.hour   = 19U;
    g_RtcTime.minute = 0;
    g_RtcTime.second = 0;

    /* Set RTC time to default */
    RTC_SetDatetime(RTC, &g_RtcTime);

    /* Enable RTC second interrupt */
    RTC_EnableInterrupts(RTC, kRTC_SecondInterruptEnable);

    /* Enable at the NVIC */
    NVIC_EnableIRQ(RTC_SEC_IRQn);

    g_RtcSecondFlag = 0;
    while (1)
    {
        if (g_RtcSecondFlag)
        {
            g_RtcSecondFlag = 0;
            /* Get data */
            RTC_GetDatetime(RTC, &g_RtcTime);
            /* Show data */
            PRINTF("Current datetime: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", g_RtcTime.year, g_RtcTime.month,
                   g_RtcTime.day, g_RtcTime.hour, g_RtcTime.minute, g_RtcTime.second);
        }
    }
}

void RTC_SEC_IRQHandler(void)
{
    if (RTC_GetStatusFlags(RTC) & kRTC_SecondInterruptFlag)
    {
        g_RtcSecondFlag = 1;
        /* Clear second interrupt flag */
        RTC_ClearStatusFlags(RTC, kRTC_SecondInterruptFlag);
    }
    SDK_ISR_EXIT_BARRIER;
}
