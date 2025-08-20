/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tickless_hal.h"
#include "fsl_rtc.h"

/**
 * @file tickless_timer_rtc_hal.c
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

#define configRTC_CLOCK_HZ (1000)

#if configUSE_TICKLESS_IDLE == 2

/* Initialize the timer hardware */
static void rtc_init(void)
{
    rtc_config_t rtcConfig;

    /* Initialize RTC module */
    RTC_GetDefaultConfig(&rtcConfig);
    RTC_Init(BOARD_RTC_TIMER_INSTANCE, &rtcConfig);
}

/* Get timer base address */
static void *rtc_get_timer_base(void)
{
    return BOARD_RTC_TIMER_INSTANCE;
}

/* Get timer IRQ number */
static IRQn_Type rtc_get_timer_irqn(void)
{
    return BOARD_RTC_TIMER_IRQ;
}

/* Start the timer with given value */
static void rtc_start_timer(void *base, uint32_t value)
{
    RTC_Type *rtcBase = (RTC_Type *)base;

    /* Set wakeup count */
    RTC_SetWakeupCount(rtcBase, value);

    /* Enable RTC */
    RTC_StartTimer(rtcBase);
}

/* Stop the timer */
static void rtc_stop_timer(void *base)
{
    RTC_Type *rtcBase = (RTC_Type *)base;

    /* Stop timer */
    RTC_StopTimer(rtcBase);
}

/* Get current timer count */
static uint32_t rtc_get_current_count(void *base)
{
    RTC_Type *rtcBase = (RTC_Type *)base;
    return RTC_GetWakeupCount(rtcBase);
}

/* Set timer period/match value */
static void rtc_set_timer_period(void *base, uint32_t value)
{
    RTC_Type *rtcBase = (RTC_Type *)base;
    RTC_SetWakeupCount(rtcBase, value);
}

/* Clear timer status flags */
static void rtc_clear_status_flags(void *base)
{
    RTC_Type *rtcBase = (RTC_Type *)base;
    RTC_ClearStatusFlags(rtcBase, kRTC_WakeupFlag);
}

/* Timer-specific ISR handler */
static void rtc_isr_handler(void)
{
    /* Clear interrupt flag */
    RTC_ClearStatusFlags(BOARD_RTC_TIMER_INSTANCE, kRTC_WakeupFlag);
}

/* Get timer clock frequency */
static uint32_t rtc_get_timer_frequency(void)
{
    return configRTC_CLOCK_HZ;
}

/* Calculate elapsed ticks based on current timer state */
static uint32_t rtc_calculate_elapsed_ticks(void *base, uint32_t reload_value, uint32_t expected_idle_time)
{
    RTC_Type *rtcBase      = (RTC_Type *)base;
    uint32_t current_count = RTC_GetWakeupCount(rtcBase);

    /* In RTC, the counter counts down from reload_value to 0 */
    uint32_t elapsed_counts = reload_value - current_count;

    /* Calculate timer counts for one tick */
    uint32_t timer_counts_for_one_tick = configRTC_CLOCK_HZ / configTICK_RATE_HZ;

    /* Calculate complete tick periods */
    uint32_t complete_tick_periods = elapsed_counts / timer_counts_for_one_tick;

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

/* RTC IRQ handler */
void RTC_IRQHandler(void)
{
    /* Call the common timer ISR handler */
    vPortTimerISRHandler();
}

/* RTC HAL implementation */
static const tickless_timer_hal_t rtc_hal = {.init                    = rtc_init,
                                             .get_timer_base          = rtc_get_timer_base,
                                             .get_timer_irqn          = rtc_get_timer_irqn,
                                             .start_timer             = rtc_start_timer,
                                             .stop_timer              = rtc_stop_timer,
                                             .get_current_count       = rtc_get_current_count,
                                             .set_timer_period        = rtc_set_timer_period,
                                             .clear_status_flags      = rtc_clear_status_flags,
                                             .isr_handler             = rtc_isr_handler,
                                             .get_timer_frequency     = rtc_get_timer_frequency,
                                             .calculate_elapsed_ticks = rtc_calculate_elapsed_ticks};

#if defined(CONFIG_TICKLESS_TIMER_RTC)
const tickless_timer_hal_t *tickless_get_timer_hal(void)
{
    return &rtc_hal;
}
#endif

#endif /* configUSE_TICKLESS_IDLE == 2 */
