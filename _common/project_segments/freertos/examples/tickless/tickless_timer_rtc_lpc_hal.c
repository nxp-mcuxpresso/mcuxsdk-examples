/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tickless_hal.h"
#include "fsl_rtc.h"
#include "fsl_power.h"
#include "fsl_clock.h"
#include "app.h"

/**
 * @file tickless_timer_rtc_lpc_hal.c
 * @brief RTC LPC HAL implementation for FreeRTOS tickless mode
 *
 * This file implements the tickless_timer_hal_t interface for the LPC RTC peripheral.
 *
 * @note This implementation requires the following board-specific macros to be defined
 * in the example's app.h file:
 *
 * - BOARD_RTC_TIMER_INSTANCE: The RTC peripheral instance to use (e.g., RTC)
 * - BOARD_RTC_TIMER_IRQ: The IRQ number for the RTC peripheral (e.g., RTC_IRQn)
 *
 * Example definitions in app.h:
 * @code
 * #define BOARD_RTC_TIMER_INSTANCE RTC
 * #define BOARD_RTC_TIMER_IRQ     RTC_IRQn
 * @endcode
 */

#define configRTC_CLOCK_HZ (1000)

#if configUSE_TICKLESS_IDLE == 2

/* Initialize the timer hardware */
static void rtc_lpc_init(void)
{
    /* Enable clock for RTC */
    CLOCK_EnableClock(kCLOCK_Rtc);

    /* Initialize RTC timer */
    RTC_Init(BOARD_RTC_TIMER_INSTANCE);

    /* Enable RTC wake timer interrupt */
    RTC_EnableInterrupts(BOARD_RTC_TIMER_INSTANCE, RTC_CTRL_WAKE1KHZ_MASK);

    /* Enable RTC interrupt in NVIC */
    EnableIRQ(BOARD_RTC_TIMER_IRQ);
}

/* Get timer base address */
static void *rtc_lpc_get_timer_base(void)
{
    return BOARD_RTC_TIMER_INSTANCE;
}

/* Get timer IRQ number */
static IRQn_Type rtc_lpc_get_timer_irqn(void)
{
    return BOARD_RTC_TIMER_IRQ;
}

/* Start the timer with given value */
static void rtc_lpc_start_timer(void *base)
{
    RTC_Type *rtcBase = (RTC_Type *)base;

    RTC_StartTimer(rtcBase);
}

/* Stop the timer */
static void rtc_lpc_stop_timer(void *base)
{
    RTC_Type *rtcBase = (RTC_Type *)base;

    RTC_StopTimer(rtcBase);
}

/* Get current timer count */
static uint32_t rtc_lpc_get_current_count(void *base)
{
    RTC_Type *rtcBase = (RTC_Type *)base;

    /* Return current wake timer count */
    return RTC_GetWakeupCount(rtcBase);
}

/* Set timer period/match value */
static void rtc_lpc_set_timer_period(void *base, uint32_t value)
{
    RTC_Type *rtcBase = (RTC_Type *)base;

    /* Set wake timer count value */
    RTC_SetWakeupCount(rtcBase, value);
}

/* Clear timer status flags */
static void rtc_lpc_clear_status_flags(void *base)
{
    RTC_Type *rtcBase = (RTC_Type *)base;

    RTC_ClearStatusFlags(rtcBase, kRTC_WakeupFlag);
}

/* Timer-specific ISR handler */
static void rtc_lpc_isr_handler(void)
{
    RTC_Type *rtcBase = (RTC_Type *)BOARD_RTC_TIMER_INSTANCE;

    /* Clear interrupt flag */
    RTC_ClearStatusFlags(rtcBase, kRTC_WakeupFlag);
}

/* Get timer clock frequency */
static uint32_t rtc_lpc_get_timer_frequency(void)
{
    /* LPC RTC wake timer runs at 1kHz */
    return configRTC_CLOCK_HZ;
}

/* Calculate elapsed ticks based on current timer state */
static uint32_t rtc_lpc_calculate_elapsed_ticks(void *base, uint32_t reload_value, uint32_t expected_idle_time)
{
    RTC_Type *rtcBase      = (RTC_Type *)base;
    uint32_t current_count = rtcBase->WAKE;

    /* For LPC RTC wake timer, it counts down from reload_value */
    uint32_t elapsed_counts;
    if (reload_value >= current_count)
    {
        elapsed_counts = reload_value - current_count;
    }
    else
    {
        /* Handle potential overflow case */
        elapsed_counts = 0;
    }

    /* Calculate timer counts for one tick */
    uint32_t timer_counts_for_one_tick = rtc_lpc_get_timer_frequency() / configTICK_RATE_HZ;

    /* Avoid division by zero */
    if (timer_counts_for_one_tick == 0)
    {
        timer_counts_for_one_tick = 1;
    }

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

/* RTC LPC HAL implementation */
static const tickless_timer_hal_t rtc_lpc_hal = {.init                    = rtc_lpc_init,
                                                 .get_timer_base          = rtc_lpc_get_timer_base,
                                                 .get_timer_irqn          = rtc_lpc_get_timer_irqn,
                                                 .start_timer             = rtc_lpc_start_timer,
                                                 .stop_timer              = rtc_lpc_stop_timer,
                                                 .get_current_count       = rtc_lpc_get_current_count,
                                                 .set_timer_period        = rtc_lpc_set_timer_period,
                                                 .clear_status_flags      = rtc_lpc_clear_status_flags,
                                                 .isr_handler             = rtc_lpc_isr_handler,
                                                 .get_timer_frequency     = rtc_lpc_get_timer_frequency,
                                                 .calculate_elapsed_ticks = rtc_lpc_calculate_elapsed_ticks};

#if defined(CONFIG_TICKLESS_TIMER_RTC_LPC)
const tickless_timer_hal_t *tickless_get_timer_hal(void)
{
    return &rtc_lpc_hal;
}
#endif

#endif /* configUSE_TICKLESS_IDLE == 2 */
