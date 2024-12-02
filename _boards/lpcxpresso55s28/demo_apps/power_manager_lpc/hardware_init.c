/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_rtc.h"
#include "fsl_debug_console.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void RTC_IRQHandler(void)
{
}
void BOARD_InitHardware(void)
{
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    CLOCK_EnableClock(kCLOCK_Gpio0); /* Enable the clock for GPIO0. */

    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    manage_evk_io_optimization();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
void DEMO_PreLowPower(void)
{
    /*!< Nothing to do */
}
void DEMO_PowerDownWakeup(void)
{
    BOARD_BootClockFRO12M();
    BOARD_InitDebugConsole();
}
void DEMO_PreDeepPowerDown(void)
{
    uint32_t sec = 0;
    uint32_t currSeconds;
    uint8_t index = 0;
    rtc_datetime_t date;

    /* Set a start date time and start RT */
    date.year   = 2018U;
    date.month  = 12U;
    date.day    = 25U;
    date.hour   = 19U;
    date.minute = 0;
    date.second = 0;

    /* Get alarm time from user */
    PRINTF("Please input the number of second to wait for waking up\r\n");
    PRINTF("The second must be positive value\r\n");
    while (index != 0x0D)
    {
        index = GETCHAR();
        if ((index >= '0') && (index <= '9'))
        {
            PUTCHAR(index);
            sec = sec * 10 + (index - 0x30U);
        }
    }
    PRINTF("\r\n");

    POWER_EnablePD(kPDRUNCFG_PD_XTAL32K); /*!< Powered down the XTAL 32 kHz RTC oscillator */
    POWER_DisablePD(kPDRUNCFG_PD_FRO32K); /*!< Powered the FRO 32 kHz RTC oscillator */
    CLOCK_AttachClk(kFRO32K_to_OSC32K);   /*!< Switch OSC32K to FRO32K */

    /* Init RTC */
    RTC_Init(RTC);

    /* RTC time counter has to be stopped before setting the date & time in the TSR register */
    RTC_StopTimer(RTC);

    /* Set RTC time to default */
    RTC_SetDatetime(RTC, &date);

    /* Enable RTC alarm interrupt */
    RTC_EnableInterrupts(RTC, kRTC_AlarmInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(RTC_IRQn);

    /* Start the RTC time counter */
    RTC_StartTimer(RTC);

    /* Read the RTC seconds register to get current time in seconds */
    currSeconds = RTC->COUNT;

    /* Add alarm seconds to current time */
    currSeconds += sec;

    /* Set alarm time in seconds */
    RTC->MATCH = currSeconds;

    /* Get alarm time */
    RTC_GetAlarm(RTC, &date);

    PRINTF("System will wakeup at%02ds later\r\n", date.second);
}
/*${function:end}*/
