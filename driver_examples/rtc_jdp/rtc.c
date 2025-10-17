/*
 * Copyright 2025 NXP
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
/* RTC interrupt interval in RTC count, will be calculated based on clock source and divide */
uint32_t g_rtcCompareValue = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* RTC callback definition                                                    */
static void rtc_callback(rtc_callback_type_t type)
{
    volatile uint32_t currentRtcCompareValue = 0;
    if (type == kRTC_RTCCallback)
    {
        PRINTF("Run in RTC call back.\r\n");
#if defined(BOARD_LED_RED_GPIO)
        LED_RED_TOGGLE();
#endif
        currentRtcCompareValue = RTC_GetRTCValue(DEMO_RTC);
        RTC_SetRTCValue(DEMO_RTC, currentRtcCompareValue + g_rtcCompareValue);
    }
    if (type == kRTC_APICallback)
    {
        PRINTF("Run in API call back.\r\n");
#if defined(BOARD_LED_GREEN_GPIO)
        LED_GREEN_TOGGLE();
#endif
    }
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* RTC interrupt period is 1s, API interrupt period is 0.5s */
    uint32_t RTCInterruptPeriod = 1000000U; /* 1000000us */
    uint32_t APIInterruptPeriod = 500000U;  /* 500000us */
    /* API interrupt interval in API count, will be calculated based on clock source and divide */
    uint32_t apiCompareValue = 0U;
    uint32_t rtcClockDivide  = 0U;
    rtc_config_t rtcConfig;

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("RTC example starts.\r\n");
    PRINTF("It prints callback function running for the RTC or API interrupt.\r\n");

    /*
     * RTC default configuration
     *     config->clockSource                       = (rtc_clock_source_t)0U;
     *     config->clockDivide                       = kRTC_ClockDivide1;
     *     config->registerAcessibleInSupervisorOnly = false;
     *     config->freezeCounterInDebugMode          = true;
     *     config->enableAnalogComparatorTrigger     = false;
     */
    RTC_GetDefaultConfig(&rtcConfig);
    rtcConfig.clockSource = RTC_CLOCK_SOURCE;
    RTC_Init(DEMO_RTC, &rtcConfig);

    /* Get divide */
    rtcClockDivide = RTC_GetDivideValue(DEMO_RTC);

    /* Get RTC and API compare value */
    g_rtcCompareValue = USEC_TO_COUNT(RTCInterruptPeriod, RTC_CLOCK_FFEQUENCY) / rtcClockDivide;
    RTC_SetRTCValue(DEMO_RTC, g_rtcCompareValue);
    apiCompareValue = USEC_TO_COUNT(APIInterruptPeriod, RTC_CLOCK_FFEQUENCY) / rtcClockDivide;
    RTC_SetAPIValue(DEMO_RTC, apiCompareValue);

    RTC_EnableInterrupts(DEMO_RTC, (uint32_t)kRTC_RTCInterruptEnable);
    RTC_EnableInterrupts(DEMO_RTC, (uint32_t)kRTC_APIInterruptEnable);
    RTC_RegisterCallBack(rtc_callback);

    NVIC_EnableIRQ(DEMO_RTC_IRQn);
    RTC_EnableRTC(DEMO_RTC);
    RTC_EnableAPI(DEMO_RTC);

    while (1)
    {
    };
}
