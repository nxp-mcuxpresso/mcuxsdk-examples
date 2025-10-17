/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_rtc.h"

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
static void RTC_AlarmCallback(rtc_callback_type_t type);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief RTC callback function for alarm events
 */
static void RTC_AlarmCallback(rtc_callback_type_t type)
{
    switch (type)
    {
        case kRTC_Alarm0Callback:
            PRINTF("\r\n*** ALARM 0 occured! ***\r\n");
            break;
        case kRTC_Alarm1Callback:
            PRINTF("\r\n*** ALARM 1 occured! ***\r\n");
            break;
        default:
            break;
    }
    SDK_ISR_EXIT_BARRIER;
}

int main(void)
{
    rtc_config_t rtcConfig;
    rtc_free_run_alarm_config_t alarm0Config, alarm1Config;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("==================================================\r\n");
    PRINTF("RTC Free Running Mode Example\r\n");
    PRINTF("==================================================\r\n");

    /* Get default RTC configuration */
    RTC_GetDefaultConfig(&rtcConfig);
    /* Set to Free Running Counter mode */
    rtcConfig.operatingMode = kRTC_ModeFreeRunningCounter;
    /* Initialize RTC */
    RTC_Init(RTC, &rtcConfig);

    /* Register callback for alarm events */
    RTC_RegisterCallBack(RTC_AlarmCallback);

    /* Set initial counter value */
    RTC_SetFreeRunningCounter(RTC, INITIAL_COUNTER_VALUE);
    PRINTF("RTC initialized in Free Running Mode with initial counter value: 0.\r\n");

    /* Start the RTC timer */
    RTC_StartTimer(RTC);

    /* Configure Alarm 0 (Repeat mode) */
    alarm0Config.alarmCounter = ALARM0_COUNTER_VALUE;
    alarm0Config.mode         = kRTC_AlarmModeRepeat;
    alarm0Config.enable       = true;
    RTC_ConfigureFreeRunningAlarm(RTC, kRTC_Alarm_0, &alarm0Config);
    /* Enable RTC alarm0 interrupt */
    RTC_EnableInterrupts(RTC, kRTC_Alarm0InterruptEnable);
    EnableIRQ(RTC_ALARM0_IRQn);
    PRINTF("Alarm 0 clock will ring every 2 seconds.\r\n");

    /* Configure Alarm 1 (Single shot mode) */
    alarm1Config.alarmCounter = ALARM1_COUNTER_VALUE;
    alarm1Config.mode         = kRTC_AlarmModeSingleShot;
    alarm1Config.enable       = true;
    RTC_ConfigureFreeRunningAlarm(RTC, kRTC_Alarm_1, &alarm1Config);
    RTC_EnableInterrupts(RTC, kRTC_Alarm1InterruptEnable);
    EnableIRQ(RTC_ALARM1_IRQn);
    PRINTF("Alarm 1 clock will ring in the 5th second.\r\n");

    while (1)
    {
    }
}
