/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_rtc.h"
#include "fsl_smm.h"
#include "fsl_cmc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief RTC alarm callback function
 * @param type callback type, member of rtc_callback_type_t enum
 */
static void RTC_AlarmCallBack(rtc_callback_type_t type);

/*!
 * @brief Print current date and time
 */
static void PrintCurrentDateTime(void);

/*!
 * @brief Print key data
 * @param keyData pointer to key data array
 * @param size size of the key data array
 */
static void PrintKeyData(uint32_t *keyData, uint8_t size);

/*!
 * @brief Print key data
 * @param keyData pointer to key data array
 * @param size size of the key data array
 */
static void PrintKeyData(uint32_t *keyData, uint8_t size);

/*!
 * @brief Check if CPU is reset from tamper
 */
bool IsResetFromTamper(void);

/*!
 * @brief Enable specified CGU interrupt sources.
 * @param base CGU peripheral base address.
 * @param mask CGU interrupt mask.
 */
void CGU_EnableInterrupts(CGU_Type *base, uint32_t mask);

/*!
 * @brief Config adon reset.
 * @param base CGU peripheral base address.
 * @param mask CGU reset mask.
 */
void CGU_EnableAONReset(CGU_Type *base, uint32_t mask);

/*!
 * @brief Enable tamper fault detect.
 */
void EnableTamperFaultDetect(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
static void RTC_AlarmCallBack(rtc_callback_type_t type)
{
    switch (type)
    {
        case kRTC_Alarm0Callback:
            PrintCurrentDateTime();
            PRINTF("\r\n*** ALARM 0 occured! ***\r\n");
            break;
        case kRTC_Alarm1Callback:
            PrintCurrentDateTime();
            PRINTF("\r\n*** ALARM 1 occured! ***\r\n");
            /* Feed the watchdog */
            RTC_RefreshWatchdog(RTC);
            PRINTF("\r\n*** WATCHDOG fed! ***\r\n");
            break;
        case kRTC_Alarm2Callback:
            PrintCurrentDateTime();
            PRINTF("\r\n*** ALARM 2 occured! ***\r\n");
            break;
        case kRTC_WatchDogCallback:
            PrintCurrentDateTime();
            PRINTF("\r\n*** WATCHDOG occured! Please feed the watchdog in proper time. ***\r\n");
            break;
        default:
            break;
    }
    SDK_ISR_EXIT_BARRIER;
}

static void PrintCurrentDateTime(void)
{
    rtc_datetime_t currentDateTime;

    (void)memset(&currentDateTime, 0, sizeof(rtc_datetime_t));

    RTC_GetDatetime(RTC, &currentDateTime);

    PRINTF("Current Date/Time: %04d-%d-%d %02d:%02d:%d:%d\r\n", currentDateTime.year, currentDateTime.month,
           currentDateTime.day, currentDateTime.hour, currentDateTime.minute, currentDateTime.second,
           currentDateTime.hundredthOfSecond);
}

static void PrintKeyData(uint32_t *keyData, uint8_t size)
{
    uint8_t i = 0;
    for (i = 0; i < size; i++)
    {
        PRINTF("  Key[%d]: 0x%X\r\n", i, keyData[i]);
    }
}

bool IsResetFromTamper(void)
{
    if ((CMC_SSRS_TAMPER_MASK & CMC_GetStickySystemResetStatus(CMC)) != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CGU_EnableInterrupts(CGU_Type *base, uint32_t mask)
{
    uint32_t enabledInterrupts;

    enabledInterrupts =
        base->INT & (uint32_t)(CGU_INT_AUTO_CAL_VAL_MASK | CGU_INT_AUTO_CAL_NOT_VAL_MASK | CGU_INT_SEC_RST_IE_MASK);
    enabledInterrupts |= mask;

    base->INT = enabledInterrupts;
}

void EnableTamperFaultDetect(void)
{
    AON__SYSCON_AON->SECURITY_GLITCH_DETECT_ENABLE = SYSCON_AON_SECURITY_GLITCH_DETECT_ENABLE_SEC_GLITCH_DET_EN_MASK;
    AON__SYSCON_AON->TAMPER_FLT_EN                 = SYSCON_AON_TAMPER_FLT_EN_LC_TAMPER_FAULT_ENABLE_MASK;
}

int main(void)
{
    rtc_config_t rtcConfig;
    rtc_datetime_t dateTime;
    rtc_tamper_input_config_t tamperInputConfig;
    rtc_bcd_alarm_config_t alarm0Config = {0U};
    rtc_bcd_alarm_config_t alarm1Config = {0U};
    rtc_bcd_alarm_config_t alarm2Config = {0U};
    bool resetFromTamper                = false;
    volatile uint64_t latchedCounter    = 0;
    uint32_t storedKeyData[8]           = {0x12345678, 0x9ABCDEF0, 0x11223344, 0x55667788,
                                           0x99AABBCC, 0xDDFF2211, 0x22334455, 0x66778899};
    uint32_t readKeyData[8]             = {0};

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("==================================================\r\n");
    PRINTF("RTC Time/Date Mode Example\r\n");
    PRINTF("==================================================\r\n");

    if (IsResetFromTamper())
    {
        PRINTF("CPU is reset from tamper.\r\n");
        resetFromTamper = true;
        CMC_ClearStickySystemResetStatus(CMC, CMC_SSRS_TAMPER_MASK);
    }

    /* Get default RTC configuration */
    RTC_GetDefaultConfig(&rtcConfig);
    /* Enable watchdog */
    rtcConfig.enableWatchdog       = true;
    rtcConfig.watchdogTimeoutValue = WATCHDOG_COUNTER_VALUE;
    /* Initialize RTC */
    RTC_Init(RTC, &rtcConfig);

    /* Register callback for alarm events */
    RTC_RegisterCallBack(RTC_AlarmCallBack);

    /* Set initial date and time: 2025-05-31 21:40:00.00 */
    dateTime.year              = 2025;
    dateTime.month             = 5;
    dateTime.day               = 31;
    dateTime.hour              = 21;
    dateTime.minute            = 40;
    dateTime.second            = 0;
    dateTime.hundredthOfSecond = 0;
    /* Saturday */
    dateTime.dayOfWeek = 6;

    RTC_SetDatetime(RTC, &dateTime);
    /* Start the RTC timer */
    RTC_StartTimer(RTC);
    /* Enable RTC watchdog interrupt */
    RTC_EnableInterrupts(RTC, kRTC_WatchdogInterruptEnable);
    EnableIRQ(RTC_WDT_IRQn);
    /* 2 RTC counts (20mSec) must separate between a write and a read. */
    SDK_DelayAtLeastUs(RTC_DELAY_TIME_US, SystemCoreClock);

    /* Enable tamper0 */
    tamperInputConfig.enableTamperInput = true;
    tamperInputConfig.polarity          = kRTC_TamperPolarityHigh;
    tamperInputConfig.filter            = kRTC_TamperFilterEnabled;
    tamperInputConfig.pullup            = kRTC_TamperPullupDisabled;
    RTC_ConfigureTamperInput(RTC, kRTC_TamperInput0, &tamperInputConfig);
    /* Enable tamper0 interrupt */
    RTC_EnableInterrupts(RTC, kRTC_Tamper0InterruptEnable);
    /* Enable system reset for tamper occurs */
    /* Enable AON domain reset */
    RESET_SetPeripheralReset(SECURITY_RESET);
    /* Enable main domain reset */
    EnableTamperFaultDetect();
    /* If reset from tamper */
    if (resetFromTamper)
    {
        /* read key data, should all be 0. */
        RTC_ReadKey(RTC, readKeyData);
        PRINTF(
            "Since the tamper occurs, the 256 bit security key data has been erased. Here is the 128 bit key data "
            "read from RTC:\r\n");
        PrintKeyData(readKeyData, sizeof(readKeyData) / sizeof(readKeyData[0]));

        /* Get the time stamp that tamper occured. */
        RTC_GetTamperTimestamp(RTC, (uint64_t *)&latchedCounter);
        /* Extract hundredths (7 bits) */
        uint8_t hundredth = (uint8_t)(latchedCounter & 0x7F);
        /* Extract seconds (6 bits) */
        uint8_t second = (uint8_t)((latchedCounter >> 7) & 0x3F);
        /* Extract minutes (6 bits) */
        uint8_t minute = (uint8_t)((latchedCounter >> 13) & 0x3F);
        /* Extract hours (5 bits) */
        uint8_t hour = (uint8_t)((latchedCounter >> 19) & 0x1F);
        /* Extract day (5 bits) */
        uint8_t day = (uint8_t)(((latchedCounter >> 24) & 0x1F) + 1);
        /* Extract month (4 bits) */
        uint8_t month = (uint8_t)(((latchedCounter >> 29) & 0x0F) + 1);
        /* Extract year (7 bits) and add 2000 offset */
        uint16_t year = 2000 + ((uint8_t)((latchedCounter >> 33) & 0x7F));
        /* Print formatted timestamp */
        PRINTF("*** TAMPER 0 timestamp: %04d-%d-%d %02d:%02d:%d:%d ***\r\n\r\n", year, month, day, hour, minute, second,
               hundredth);
    }

    PrintCurrentDateTime();

    PRINTF("Tamper 0 is enabled, press the button SW6 to trigger it.\r\n");

    /* Write key data */
    RTC_WriteKey(RTC, storedKeyData);
    PRINTF("Stored 256 bit key data written to RTC:\r\n");
    PrintKeyData(storedKeyData, sizeof(storedKeyData) / sizeof(storedKeyData[0]));
    /* Read key data */
    RTC_ReadKey(RTC, readKeyData);
    PRINTF("Relevant 128 bit key data read from RTC:\r\n");
    PrintKeyData(readKeyData, sizeof(readKeyData) / sizeof(readKeyData[0]));

    /* Configure Alarm 0 - Trigger at 21:40:04 (Single shot) */
    alarm0Config.hour      = 21;
    alarm0Config.minute    = 40;
    alarm0Config.second    = 4;
    alarm0Config.day       = 31;
    alarm0Config.dayOfWeek = 6;
    alarm0Config.month     = 5;
    alarm0Config.mask      = kRTC_AlarmMaskIgnoreNothing; /* Match no fields */
    alarm0Config.mode      = kRTC_AlarmModeSingleShot;
    alarm0Config.enable    = true;
    RTC_ConfigureBCDAlarm(RTC, kRTC_Alarm_0, &alarm0Config);
    /* Enable RTC alarm 0 interrupt */
    RTC_EnableInterrupts(RTC, kRTC_Alarm0InterruptEnable);
    EnableIRQ(RTC_ALARM0_IRQn);
    PRINTF("Alarm 0 will ring at 21:40:04.\r\n");

    /* Configure Alarm 1 - Trigger every second at 80th hundredth */
    alarm1Config.hour              = 0;
    alarm1Config.minute            = 0;
    alarm1Config.second            = 0;
    alarm1Config.hundredthOfSecond = 80;
    alarm1Config.day               = 0;
    alarm1Config.dayOfWeek         = 0;
    alarm1Config.month             = 0;
    alarm1Config.mask              = (uint8_t)kRTC_AlarmMaskIgnoreAll;
    alarm1Config.mode              = kRTC_AlarmModeRepeat;
    alarm1Config.enable            = true;
    RTC_ConfigureBCDAlarm(RTC, kRTC_Alarm_1, &alarm1Config);
    /* Enable RTC alarm1 interrupt */
    RTC_EnableInterrupts(RTC, kRTC_Alarm1InterruptEnable);
    EnableIRQ(RTC_ALARM1_IRQn);
    PRINTF("Alarm 1 will ring every second at 80.\r\n");

    /* Configure Alarm 2 - Trigger every minute at 2nd second 60th hundredth */
    alarm2Config.hour              = 0;
    alarm2Config.minute            = 0;
    alarm2Config.second            = 2;
    alarm2Config.hundredthOfSecond = 60;
    alarm2Config.day               = 0;
    alarm2Config.dayOfWeek         = 0;
    alarm2Config.month             = 0;
    alarm2Config.mask   = (uint8_t)(kRTC_AlarmMaskIgnoreHour | kRTC_AlarmMaskIgnoreMinute | kRTC_AlarmMaskIgnoreDay |
                                  kRTC_AlarmMaskIgnoreDayOfWeek | kRTC_AlarmMaskIgnoreMonth);
    alarm2Config.mode   = kRTC_AlarmModeRepeat;
    alarm2Config.enable = true;
    RTC_ConfigureBCDAlarm(RTC, kRTC_Alarm_2, &alarm2Config);
    /* Enable RTC alarm2 interrupt */
    RTC_EnableInterrupts(RTC, kRTC_Alarm2InterruptEnable);
    EnableIRQ(RTC_ALARM2_IRQn);
    PRINTF("Alarm 2 will ring every minute at 2nd second 60 hundredth.\r\n");

    while (1)
    {
    }
}
