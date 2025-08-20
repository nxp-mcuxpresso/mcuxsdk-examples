/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file tickless_timer_rtc_jdp_hal.c
 * @brief RTC_JDP HAL implementation for FreeRTOS tickless mode
 *
 * This file implements the tickless_timer_hal_t interface for the RTC_JDP peripheral.
 *
 * @note This implementation requires the following board-specific macros to be defined
 * in the example's app.h file:
 *
 * - BOARD_RTC_TIMER_INSTANCE: The RTC_JDP peripheral instance to use (e.g., RTC)
 * - BOARD_RTC_TIMER_IRQ: The IRQ number for the RTC_JDP peripheral (e.g., RTC_IRQn)
 * - BOARD_RTC_CLOCK_FREQUENCY: The clock frequency of the RTC_JDP in Hz (e.g., 32768)
 * - BOARD_RTC_CLOCK_SOURCE: The clock source for the RTC_JDP (e.g., kRTC_ClockSource_LPO)
 *
 * Example definitions in app.h:
 * @code
 * #define BOARD_RTC_TIMER_INSTANCE RTC
 * #define BOARD_RTC_TIMER_IRQ RTC_IRQn
 * #define BOARD_RTC_CLOCK_FREQUENCY CLOCK_GetFreq(kCLOCK_SircClk)
 * #define BOARD_RTC_CLOCK_SOURCE kRTC_SIRCClock
 * @endcode
 */

#include "tickless_hal.h"
#include "fsl_rtc.h"

#define configRTC_CLOCK_HZ (1000)

#if configUSE_TICKLESS_IDLE == 2

/* Initialize the timer hardware */
static void rtc_jdp_init(void)
{
    rtc_config_t rtcConfig;

    /* Initialize RTC module */
    RTC_GetDefaultConfig(&rtcConfig);

    /* Configure RTC for tickless operation */
    rtcConfig.clockSource = BOARD_RTC_CLOCK_SOURCE;

    RTC_Init(BOARD_RTC_TIMER_INSTANCE, &rtcConfig);

    /* Enable RTC interrupt */
    RTC_EnableInterrupts(BOARD_RTC_TIMER_INSTANCE, kRTC_RTCInterruptEnable);
}

/* Get timer base address */
static void *rtc_jdp_get_timer_base(void)
{
    return BOARD_RTC_TIMER_INSTANCE;
}

/* Get timer IRQ number */
static IRQn_Type rtc_jdp_get_timer_irqn(void)
{
    return BOARD_RTC_TIMER_IRQ;
}

/* Start the timer with given value */
static void rtc_jdp_start_timer(void *base)
{
    RTC_Type *rtcBase = (RTC_Type *)base;

    /* Enable RTC */
    RTC_EnableRTC(rtcBase);
}

/* Stop the timer */
static void rtc_jdp_stop_timer(void *base)
{
    RTC_Type *rtcBase = (RTC_Type *)base;

    /* Disable RTC */
    RTC_DisableRTC(rtcBase);
}

/* Get current timer count */
static uint32_t rtc_jdp_get_current_count(void *base)
{
    RTC_Type *rtcBase = (RTC_Type *)base;
    return RTC_GetCountValue(rtcBase);
}

/* Set timer period/match value */
static void rtc_jdp_set_timer_period(void *base, uint32_t value)
{
    RTC_Type *rtcBase = (RTC_Type *)base;

    /* Ensure minimum compare value */
    if (value <= MINIMUM_RTCVAL)
    {
        value = MINIMUM_RTCVAL + 1;
    }

    /* Get divide */
    const uint32_t rtcClockDivide = RTC_GetDivideValue(rtcBase);

    /* Get RTC and API compare value */
    uint32_t rtcCompareValue = USEC_TO_COUNT(value * 1000, BOARD_RTC_CLOCK_FFEQUENCY) / rtcClockDivide;

    RTC_SetRTCValue(rtcBase, rtcCompareValue);
}

/* Clear timer status flags */
static void rtc_jdp_clear_status_flags(void *base)
{
    RTC_Type *rtcBase = (RTC_Type *)base;
    RTC_ClearInterruptFlags(rtcBase, kRTC_RTCInterruptFlag);
}

/* Timer-specific ISR handler */
static void rtc_jdp_isr_handler(void)
{
    /* Clear interrupt flag */
    RTC_ClearInterruptFlags(BOARD_RTC_TIMER_INSTANCE, kRTC_RTCInterruptFlag);
}

/* Get timer clock frequency */
static uint32_t rtc_jdp_get_timer_frequency(void)
{
    return configRTC_CLOCK_HZ;
}

/* Calculate elapsed ticks based on current timer state */
static uint32_t rtc_jdp_calculate_elapsed_ticks(void *base, uint32_t reload_value, uint32_t expected_idle_time)
{
    RTC_Type *rtcBase      = (RTC_Type *)base;
    uint32_t current_count = RTC_GetCountValue(rtcBase);
    uint32_t compare_value = RTC_GetRTCValue(rtcBase);

    /* Get divide value */
    const uint32_t rtcClockDivide = RTC_GetDivideValue(rtcBase);

    /* Calculate elapsed counts */
    uint32_t elapsed_counts;

    /* In RTC_JDP, the counter counts up from 0 */
    if (current_count < compare_value)
    {
        /* Timer hasn't reached the compare value yet */
        elapsed_counts = current_count;
    }
    else
    {
        /* Timer has reached or exceeded the compare value */
        elapsed_counts = compare_value;
    }

    /* Convert elapsed counts back to microseconds */
    uint32_t elapsed_usec = COUNT_TO_USEC(elapsed_counts * rtcClockDivide, BOARD_RTC_CLOCK_FFEQUENCY) / 1000;

    /* Calculate timer counts for one tick in microseconds */
    uint32_t usec_for_one_tick = 1000000 / configTICK_RATE_HZ;

    /* Calculate complete tick periods */
    uint32_t complete_tick_periods = elapsed_usec / usec_for_one_tick;

    /* Safety check with explicit bounds checking */
    uint32_t max_allowed_ticks = 0U;

    if (expected_idle_time > 1U)
    {
        /* Safe subtraction since expected_idle_time > 1 */
        max_allowed_ticks = expected_idle_time - 1U;
    }

    /* Safety check */
    if (complete_tick_periods > max_allowed_ticks)
    {
        complete_tick_periods = max_allowed_ticks;
    }

    return complete_tick_periods;
}

/* RTC_JDP HAL implementation */
static const tickless_timer_hal_t rtc_jdp_hal = {.init                    = rtc_jdp_init,
                                                 .get_timer_base          = rtc_jdp_get_timer_base,
                                                 .get_timer_irqn          = rtc_jdp_get_timer_irqn,
                                                 .start_timer             = rtc_jdp_start_timer,
                                                 .stop_timer              = rtc_jdp_stop_timer,
                                                 .get_current_count       = rtc_jdp_get_current_count,
                                                 .set_timer_period        = rtc_jdp_set_timer_period,
                                                 .clear_status_flags      = rtc_jdp_clear_status_flags,
                                                 .isr_handler             = rtc_jdp_isr_handler,
                                                 .get_timer_frequency     = rtc_jdp_get_timer_frequency,
                                                 .calculate_elapsed_ticks = rtc_jdp_calculate_elapsed_ticks};

#if defined(CONFIG_TICKLESS_TIMER_RTC_JDP)
const tickless_timer_hal_t *tickless_get_timer_hal(void)
{
    return &rtc_jdp_hal;
}
#endif

#endif /* configUSE_TICKLESS_IDLE == 2 */
