/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tickless_hal.h"

#if configUSE_TICKLESS_IDLE == 2
#include "fsl_ostimer.h"
#else
#include "fsl_device_registers.h"
#endif

/**
 * @file tickless_timer_ostimer_hal.c
 * @brief OSTIMER HAL implementation for FreeRTOS tickless mode
 *
 * This file implements the tickless_timer_hal_t interface for the OS Timer (OSTIMER) peripheral.
 * OSTIMER is a 64-bit timer that requires special handling for the 32-bit HAL interface.
 *
 * @note This implementation requires the following board-specific macros to be defined
 * in the examples's app.h file:
 *
 * - BOARD_OSTIMER_TIMER_INSTANCE: The OSTIMER peripheral instance to use (e.g., OSTIMER)
 * - BOARD_OSTIMER_TIMER_IRQ: The IRQ number for the OSTIMER peripheral (e.g., OS_EVENT_IRQn)
 *
 * Example definitions in app.h:
 * @code
 * #define BOARD_OSTIMER_TIMER_INSTANCE OSTIMER
 * #define BOARD_OSTIMER_TIMER_IRQ OS_EVENT_IRQn
 * @endcode
 *
 */

#define configOSTIMER_CLOCK_HZ (32768)

#if configUSE_TICKLESS_IDLE == 2

/* OSTIMER-specific state - minimal state needed for elapsed time calculation */
static uint64_t ostimer_sleep_start_time           = 0;
static uint32_t ostimer_stopped_timer_compensation = 0;
static bool ostimer_was_started                    = false;

/* OSTIMER callback for match interrupt */
static void ostimer_match_callback(void)
{
    /* The interrupt flag will be cleared by the common ISR handler */
    /* This callback is just to handle the OSTIMER-specific interrupt mechanism */
}

/* Initialize the timer hardware */
static void ostimer_init(void)
{
    /* Calculate OSTIMER-specific compensation factor */
    ostimer_stopped_timer_compensation = 45UL * (configCPU_CLOCK_HZ / configOSTIMER_CLOCK_HZ);

    /* Reset start time */
    ostimer_sleep_start_time = 0;
}

/* Get timer base address */
static void *ostimer_get_timer_base(void)
{
    return BOARD_OSTIMER_TIMER_INSTANCE;
}

/* Get timer IRQ number */
static IRQn_Type ostimer_get_timer_irqn(void)
{
    return BOARD_OSTIMER_TIMER_IRQ;
}

/* Start the timer */
static void ostimer_start_timer(void *base)
{
    (void)base;

    /* OS Timer Does not support start - start is handled in
     * ostimer_set_timer_period() function.
     * This will just indicate flag that timer was started so timer_stop will
     * know if it should stop the timer.
     */
    ostimer_was_started = true;
}

/* Stop the timer */
static void ostimer_stop_timer(void *base)
{
    OSTIMER_Type *ostimerBase = (OSTIMER_Type *)base;

    if (ostimer_was_started)
    {
        /* Stop OSTIMER to avoid further interrupts */
        OSTIMER_Deinit(ostimerBase);
        ostimer_was_started = false;
    }
}

/* Get current timer count (lower 32 bits) */
static uint32_t ostimer_get_current_count(void *base)
{
    OSTIMER_Type *ostimerBase = (OSTIMER_Type *)base;
    uint64_t current_value    = OSTIMER_GetCurrentTimerValue(ostimerBase);

    /* Return the lower 32 bits for compatibility with HAL interface */
    return (uint32_t)(current_value & 0xFFFFFFFFUL);
}

/* Set timer period/match value */
static void ostimer_set_timer_period(void *base, uint32_t value)
{
    /**
     * For the OSTimer this function will serve also as Timer Start
     * So timer_start will be left empty.
     */

    OSTIMER_Type *ostimerBase = (OSTIMER_Type *)base;

    /* Initialize OSTIMER */
    OSTIMER_Init(ostimerBase);
    ostimer_was_started = true;

    /* Clear any pending interrupt flags */
    OSTIMER_ClearStatusFlags(ostimerBase, kOSTIMER_MatchInterruptFlag);

    /* Apply OSTIMER-specific compensation for startup/shutdown latency */
    uint32_t compensated_value = value;
    if (ostimer_stopped_timer_compensation > 0 && value > ostimer_stopped_timer_compensation)
    {
        compensated_value = value - ostimer_stopped_timer_compensation;
    }

    /* Store the start time for later elapsed time calculation */
    ostimer_sleep_start_time = OSTIMER_GetCurrentTimerValue(ostimerBase);

    /* Set match value relative to current time with compensation applied */
    OSTIMER_SetMatchValue(ostimerBase, (ostimer_sleep_start_time + compensated_value), ostimer_match_callback);
}

/* Clear timer status flags */
static void ostimer_clear_status_flags(void *base)
{
    (void)base;

    /* Not Implemented - flags are cleared in ostimer_isr_handler */
}

/* Timer-specific ISR handler */
static void ostimer_isr_handler(void)
{
    /* Clear interrupt flag */
    OSTIMER_ClearStatusFlags(BOARD_OSTIMER_TIMER_INSTANCE, kOSTIMER_MatchInterruptFlag);
}

/* Get timer clock frequency */
static uint32_t ostimer_get_timer_frequency(void)
{
    return configOSTIMER_CLOCK_HZ;
}

/* Calculate elapsed ticks based on current timer state */
static uint32_t ostimer_calculate_elapsed_ticks(void *base, uint32_t reload_value, uint32_t expected_idle_time)
{
    OSTIMER_Type *ostimerBase      = (OSTIMER_Type *)base;
    uint32_t complete_tick_periods = 0;

    /* Check if the match interrupt occurred */
    uint32_t status = OSTIMER_GetStatusFlags(ostimerBase);

    /* Safety check with explicit bounds checking */
    uint32_t max_allowed_ticks = 0U;

    if (expected_idle_time > 1U)
    {
        /* Safe subtraction since expected_idle_time > 1 */
        max_allowed_ticks = expected_idle_time - 1U;
    }

    if (status & kOSTIMER_MatchInterruptFlag)
    {
        /* Timer expired - we slept for the full expected time minus 1
         * (since the tick interrupt will be processed after this function returns) */
        complete_tick_periods = max_allowed_ticks;
    }
    else
    {
        /* Timer didn't expire - calculate elapsed time using stored start time */
        if (ostimer_sleep_start_time != 0)
        {
            uint64_t current_value     = OSTIMER_GetCurrentTimerValue(ostimerBase);
            uint64_t elapsed_counts_64 = current_value - ostimer_sleep_start_time;
            uint32_t elapsed_counts;

            /* Check if the elapsed time fits in a 32-bit value */
            if (elapsed_counts_64 > UINT32_MAX)
            {
                /* Handle overflow - limit to maximum value */
                elapsed_counts = UINT32_MAX;
            }
            else
            {
                elapsed_counts = (uint32_t)elapsed_counts_64;
            }

            /* Calculate timer counts for one tick */
            uint32_t timer_counts_for_one_tick = configOSTIMER_CLOCK_HZ / configTICK_RATE_HZ;

            /* How many complete tick periods passed while the processor was sleeping? */
            if (timer_counts_for_one_tick > 0)
            {
                complete_tick_periods = elapsed_counts / timer_counts_for_one_tick;
            }
            else
            {
                complete_tick_periods = 0;
            }
        }
        else
        {
            /* Start time not captured, assume minimal elapsed time */
            complete_tick_periods = 0;
        }
    }

    /* Safety check */
    if (complete_tick_periods > max_allowed_ticks)
    {
        complete_tick_periods = max_allowed_ticks;
    }

    return complete_tick_periods;
}

/* OSTIMER HAL implementation */
static const tickless_timer_hal_t ostimer_hal = {.init                    = ostimer_init,
                                                 .get_timer_base          = ostimer_get_timer_base,
                                                 .get_timer_irqn          = ostimer_get_timer_irqn,
                                                 .start_timer             = ostimer_start_timer,
                                                 .stop_timer              = ostimer_stop_timer,
                                                 .get_current_count       = ostimer_get_current_count,
                                                 .set_timer_period        = ostimer_set_timer_period,
                                                 .clear_status_flags      = ostimer_clear_status_flags,
                                                 .isr_handler             = ostimer_isr_handler,
                                                 .get_timer_frequency     = ostimer_get_timer_frequency,
                                                 .calculate_elapsed_ticks = ostimer_calculate_elapsed_ticks};

#if defined(CONFIG_TICKLESS_TIMER_OSTIMER)
const tickless_timer_hal_t *tickless_get_timer_hal(void)
{
    return &ostimer_hal;
}
#endif

#endif /* configUSE_TICKLESS_IDLE == 2 */
