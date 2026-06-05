/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "app.h"
#include "fsl_bbnsm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SECONDS_IN_A_DAY    (86400U)
#define SECONDS_IN_A_HOUR   (3600U)
#define SECONDS_IN_A_MINUTE (60U)
#define DAYS_IN_A_YEAR      (365U)
#define YEAR_RANGE_START    (1970U)
/* Written to GPR[0] on cold boot. */
#define RTC_VALID_MAGIC     (0xB0B0CA1CU)

/*! @brief Structure used to hold a calendar date and time. */
typedef struct _demo_datetime
{
    uint16_t year;  /*!< Range from 1970 to 2099. */
    uint8_t month;  /*!< Range from 1 to 12. */
    uint8_t day;    /*!< Range from 1 to 31 (depending on month). */
    uint8_t hour;   /*!< Range from 0 to 23. */
    uint8_t minute; /*!< Range from 0 to 59. */
    uint8_t second; /*!< Range from 0 to 59. */
} demo_datetime_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static uint32_t DEMO_ConvertDatetimeToSeconds(const demo_datetime_t *datetime);
static void DEMO_ConvertSecondsToDatetime(uint32_t seconds, demo_datetime_t *datetime);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_busyWait;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Converts a calendar date/time to the number of seconds since 1970-01-01 00:00:00.
 */
static uint32_t DEMO_ConvertDatetimeToSeconds(const demo_datetime_t *datetime)
{
    /* Number of days from the begin of the non leap-year till the first of each month. */
    static const uint16_t monthDays[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};
    uint32_t seconds;

    /* Compute number of days from 1970 till the given year. */
    seconds = ((uint32_t)datetime->year - YEAR_RANGE_START) * DAYS_IN_A_YEAR;
    /* Add the leap-year days. */
    seconds += (((uint32_t)datetime->year / 4U) - (YEAR_RANGE_START / 4U));
    /* Add the number of days till the given month. */
    seconds += monthDays[datetime->month];
    /* Add the days in the given month minus the current day. */
    seconds += ((uint32_t)datetime->day - 1U);
    /* For a leap-year, if the month is January or February, decrement the day counter. */
    if ((0U == (datetime->year & 3U)) && (datetime->month <= 2U) && (0U != seconds))
    {
        seconds--;
    }

    seconds = (seconds * SECONDS_IN_A_DAY) + ((uint32_t)datetime->hour * SECONDS_IN_A_HOUR) +
              ((uint32_t)datetime->minute * SECONDS_IN_A_MINUTE) + (uint32_t)datetime->second;

    return seconds;
}

/*!
 * @brief Converts a number of seconds since 1970-01-01 00:00:00 into a calendar date/time.
 */
static void DEMO_ConvertSecondsToDatetime(uint32_t seconds, demo_datetime_t *datetime)
{
    uint32_t x;
    uint32_t secondsRemaining;
    uint32_t days;
    uint16_t daysInYear;
    /* Days in a month. Index 0 is unused; valid months start from 1.
     * Static (not const): daysPerMonth[2] is always assigned 28 or 29 before use,
     * so cross-call state is safe. */
    static uint8_t daysPerMonth[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

    secondsRemaining = seconds;

    /* Number of days; add 1 for the current day represented in the hours/minutes/seconds. */
    days             = secondsRemaining / SECONDS_IN_A_DAY + 1U;
    secondsRemaining = secondsRemaining % SECONDS_IN_A_DAY;

    /* Hour, minute and second fields. */
    datetime->hour   = (uint8_t)(secondsRemaining / SECONDS_IN_A_HOUR);
    secondsRemaining = secondsRemaining % SECONDS_IN_A_HOUR;
    datetime->minute = (uint8_t)(secondsRemaining / SECONDS_IN_A_MINUTE);
    datetime->second = (uint8_t)(secondsRemaining % SECONDS_IN_A_MINUTE);

    /* Year. */
    daysInYear     = DAYS_IN_A_YEAR;
    datetime->year = YEAR_RANGE_START;
    while (days > daysInYear)
    {
        days -= daysInYear;
        datetime->year++;

        if (0U != (datetime->year & 3U))
        {
            daysInYear = DAYS_IN_A_YEAR;
        }
        else
        {
            daysInYear = DAYS_IN_A_YEAR + 1U;
        }
    }

    /* Set February length for the current year. */
    if (0U == (datetime->year & 3U))
    {
        daysPerMonth[2] = 29U;
    }
    else
    {
        daysPerMonth[2] = 28U;
    }

    /* Month. */
    for (x = 1U; x <= 12U; x++)
    {
        if (days <= daysPerMonth[x])
        {
            datetime->month = (uint8_t)x;
            break;
        }
        else
        {
            days -= daysPerMonth[x];
        }
    }

    /* Remaining days are the day-of-month. */
    datetime->day = (uint8_t)days;
}

/*!
 * @brief ISR for the BBNSM RTC alarm interrupt.
 *
 * This function clears the alarm flag and releases the busy-wait in main().
 */
void DEMO_BBNSM_IRQ_HANDLER(void)
{
    if (0U != (BBNSM_GetStatusFlags(DEMO_BBNSM_BASE) & kBBNSM_RTC_AlarmInterruptFlag))
    {
        g_busyWait = false;

        /* Clear the alarm flag (write-1-to-clear) and stop the alarm interrupt. */
        BBNSM_ClearStatusFlags(DEMO_BBNSM_BASE, kBBNSM_RTC_AlarmInterruptFlag);
        BBNSM_DisableInterrupts(DEMO_BBNSM_BASE, kBBNSM_RTC_AlarmInterrupt);
    }
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t sec;
    uint32_t currSeconds;
    uint8_t index;
    demo_datetime_t date;
    bbnsm_rtc_config_t bbnsmConfig;
    bool isColdBoot;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Read GPR[0] before any peripheral reset: if the magic value is present,
     * RTC counter is still valid. */
    isColdBoot = (DEMO_BBNSM_BASE->GPR[0] != RTC_VALID_MAGIC);

    /* Init BBNSM RTC */
    /*
     * bbnsmConfig.rtcCalEnable = false;
     * bbnsmConfig.rtcCalValue  = 0U;
     */
    BBNSM_RTC_GetDefaultConfig(&bbnsmConfig);
    BBNSM_RTC_Init(DEMO_BBNSM_BASE, &bbnsmConfig);

    if (isColdBoot)
    {
        PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
        PRINTF("BBNSM RTC example: set up time to wake up an alarm\r\n");
        /* Cold boot: set RTC to a known time. */
        date.year   = 2024U;
        date.month  = 12U;
        date.day    = 25U;
        date.hour   = 19U;
        date.minute = 0U;
        date.second = 0U;

        BBNSM_RTC_StopTimer(DEMO_BBNSM_BASE);
        BBNSM_RTC_SetSeconds(DEMO_BBNSM_BASE, DEMO_ConvertDatetimeToSeconds(&date));
        DEMO_BBNSM_BASE->GPR[0] = RTC_VALID_MAGIC;
        BBNSM_RTC_StartTimer(DEMO_BBNSM_BASE);
    }
    else
    {
        currSeconds = BBNSM_RTC_GetSeconds(DEMO_BBNSM_BASE);
        DEMO_ConvertSecondsToDatetime(currSeconds, &date);
        PRINTF("RTC survived power-off: %04hu-%02hhu-%02hhu %02hhu:%02hhu:%02hhu\r\n",
               date.year, date.month, date.day, date.hour, date.minute, date.second);
    }

    /* This loop will set the RTC alarm. */
    while (1)
    {
        index = 0U;
        sec   = 0U;

        /* Get the current time. */
        currSeconds = BBNSM_RTC_GetSeconds(DEMO_BBNSM_BASE);
        DEMO_ConvertSecondsToDatetime(currSeconds, &date);

        /* Print the current time. */
        PRINTF("Current datetime: %04hu-%02hhu-%02hhu %02hhu:%02hhu:%02hhu\r\n", date.year, date.month, date.day, date.hour,
               date.minute, date.second);

        /* Get the alarm time (in seconds) from the user. */
        PRINTF("Please input the number of seconds to wait for the alarm\r\n");
        PRINTF("The seconds must be a positive value\r\n");
        while (index != (uint8_t)'\r')
        {
            index = GETCHAR();
            if ((index >= '0') && (index <= '9'))
            {
                PUTCHAR(index);
                sec = sec * 10U + ((uint32_t)index - 0x30U);
            }
        }
        PRINTF("\r\n");

        /* Compute the absolute alarm time. */
        currSeconds = BBNSM_RTC_GetSeconds(DEMO_BBNSM_BASE) + sec;

        DisableIRQ(DEMO_BBNSM_IRQn);

        /* Set the alarm time (this also enables the BBNSM alarm interrupt). */
        if (kStatus_Success != BBNSM_RTC_SetAlarm(DEMO_BBNSM_BASE, currSeconds))
        {
            /* No alarm was armed, so leave the IRQ masked; the next successful arm
             * re-enables it. */
            PRINTF("Failed to set the alarm. Please retry with a larger value.\r\n");
            continue;
        }

        /* The new alarm time is in the future, so the compare condition is now false and
         * clearing the event flag sticks. Drop any IRQ left pending by the stale event. */
        BBNSM_ClearStatusFlags(DEMO_BBNSM_BASE, kBBNSM_RTC_AlarmInterruptFlag);
        NVIC_ClearPendingIRQ(DEMO_BBNSM_IRQn);

        g_busyWait = true;
        EnableIRQ(DEMO_BBNSM_IRQn);

        /* Print the alarm time. */
        DEMO_ConvertSecondsToDatetime(BBNSM_RTC_GetAlarm(DEMO_BBNSM_BASE), &date);
        PRINTF("Alarm will occur at: %04hu-%02hhu-%02hhu %02hhu:%02hhu:%02hhu\r\n", date.year, date.month, date.day,
               date.hour, date.minute, date.second);

        /* Wait until the alarm occurs. */
        while (g_busyWait)
        {
        }

        PRINTF("\r\n Alarm occurs !!!! \r\n");
    }
}
