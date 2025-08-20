/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file tickless_timer_lptmr_hal.c
 * @brief LPTMR HAL implementation for FreeRTOS tickless mode
 *
 * This file implements the tickless_timer_hal_t interface for the Low Power Timer (LPTMR) peripheral.
 *
 * @note This implementation requires the following board-specific macros to be defined
 * in the board's app.h file:
 *
 * - BOARD_LPTMR_TIMER_INSTANCE: The LPTMR peripheral instance to use (e.g., LPTMR0)
 * - BOARD_LPTMR_TIMER_IRQ: The IRQ number for the LPTMR peripheral (e.g., LPTMR0_IRQn)
 * - BOARD_LPTMR_CLOCK_SOURCE: The clock source for the LPTMR (e.g., kLPTMR_PrescalerClock_1)
 * - BOARD_LPTMR_BYPASS_PRESCALER: Bypass prescaler value (e.g., true/false)
 * - BOARD_LPTMR_CLOCK_FILTER: The prescaler or glitch filter value. Optional.
 *
 * Example definitions in app.h:
 * @code
 * #define BOARD_LPTMR_TIMER_INSTANCE LPTMR0
 * #define BOARD_LPTMR_TIMER_IRQ LPTMR0_IRQn
 * #define BOARD_LPTMR_CLOCK_SOURCE kLPTMR_PrescalerClock_1
 * #define BOARD_LPTMR_CLOCK_FILTER   kLPTMR_Prescale_Glitch_10
 * #define BOARD_LPTMR_BYPASS_PRESCALER true
 * #define BOARD_TIMER_IRQ_HANDLER    LPTMR0_IRQHandler
 * @endcode
 *
 * Additionally, the board's hardware_init.c file should configure the LPTMR clock source
 * if needed for proper operation.
 */

#include "tickless_hal.h"
#include "fsl_lptmr.h"

#ifndef BOARD_LPTMR_CLOCK_FILTER
#define BOARD_LPTMR_CLOCK_FILTER kLPTMR_Prescale_Glitch_0
#endif

#ifndef BOARD_LPTMR_BYPASS_PRESCALER
#define BOARD_LPTMR_BYPASS_PRESCALER false
#endif

#if configUSE_TICKLESS_IDLE == 2

/**
 * @brief Initialize the LPTMR timer hardware
 *
 * This function initializes the LPTMR module with the configuration
 * specified by the board-specific macros.
 */
static void lptmr_init(void)
{
    lptmr_config_t lptmrConfig;

    /* Initialize LPTMR module */
    LPTMR_GetDefaultConfig(&lptmrConfig);

    /* Configure LPTMR for tickless operation */
    lptmrConfig.timerMode            = kLPTMR_TimerModeTimeCounter;
    lptmrConfig.pinSelect            = kLPTMR_PinSelectInput_0;
    lptmrConfig.pinPolarity          = kLPTMR_PinPolarityActiveHigh;
    lptmrConfig.enableFreeRunning    = false;
    lptmrConfig.bypassPrescaler      = BOARD_LPTMR_BYPASS_PRESCALER;
    lptmrConfig.prescalerClockSource = BOARD_LPTMR_CLOCK_SOURCE;
    lptmrConfig.value                = BOARD_LPTMR_CLOCK_FILTER;

    LPTMR_Init(BOARD_LPTMR_TIMER_INSTANCE, &lptmrConfig);

    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(BOARD_LPTMR_TIMER_INSTANCE, kLPTMR_TimerInterruptEnable);
}

/**
 * @brief Get timer base address
 *
 * @return Base address of the LPTMR peripheral
 */
static void *lptmr_get_timer_base(void)
{
    return BOARD_LPTMR_TIMER_INSTANCE;
}

/**
 * @brief Get timer IRQ number
 *
 * @return IRQ number for the LPTMR peripheral
 */
static IRQn_Type lptmr_get_timer_irqn(void)
{
    return BOARD_LPTMR_TIMER_IRQ;
}

/**
 * @brief Start the LPTMR timer with given value
 *
 * @param base Base address of the LPTMR peripheral
 */
static void lptmr_start_timer(void *base)
{
    LPTMR_Type *lptmrBase = (LPTMR_Type *)base;

    /* Enable LPTMR interrupt */
    LPTMR_EnableInterrupts(lptmrBase, kLPTMR_TimerInterruptEnable);

    /* Start timer */
    LPTMR_StartTimer(lptmrBase);
}

/**
 * @brief Stop the LPTMR timer
 *
 * @param base Base address of the LPTMR peripheral
 */
static void lptmr_stop_timer(void *base)
{
    LPTMR_Type *lptmrBase = (LPTMR_Type *)base;

    /* Disable interrupts */
    LPTMR_DisableInterrupts(lptmrBase, kLPTMR_TimerInterruptEnable);

    /* Stop timer */
    LPTMR_StopTimer(lptmrBase);
}

/**
 * @brief Get current LPTMR timer count
 *
 * @param base Base address of the LPTMR peripheral
 * @return Current count value of the LPTMR timer
 */
static uint32_t lptmr_get_current_count(void *base)
{
    LPTMR_Type *lptmrBase = (LPTMR_Type *)base;
    return LPTMR_GetCurrentTimerCount(lptmrBase);
}

/**
 * @brief Set LPTMR timer period/match value
 *
 * @param base Base address of the LPTMR peripheral
 * @param value Period or match value to set
 */
static void lptmr_set_timer_period(void *base, uint32_t value)
{
    LPTMR_Type *lptmrBase = (LPTMR_Type *)base;
    LPTMR_SetTimerPeriod(lptmrBase, value);
}

/**
 * @brief Clear LPTMR timer status flags
 *
 * @param base Base address of the LPTMR peripheral
 */
static void lptmr_clear_status_flags(void *base)
{
    LPTMR_Type *lptmrBase = (LPTMR_Type *)base;
    LPTMR_ClearStatusFlags(lptmrBase, kLPTMR_TimerCompareFlag);
}

/**
 * @brief LPTMR timer-specific ISR handler
 *
 * This function handles LPTMR-specific operations in the ISR.
 * It clears the interrupt flag to prevent repeated interrupts.
 */
static void lptmr_isr_handler(void)
{
    /* Clear interrupt flag */
    LPTMR_ClearStatusFlags(BOARD_LPTMR_TIMER_INSTANCE, kLPTMR_TimerCompareFlag);
}

/**
 * @brief Get LPTMR timer clock frequency in Hz
 *
 * This function returns the clock frequency of the LPTMR timer.
 * The frequency depends on the clock source configuration.
 *
 * @return LPTMR timer clock frequency in Hz
 */
static uint32_t lptmr_get_timer_frequency(void)
{
    /* This should be defined based on your board's clock configuration */
    /* Common values are 32768 Hz for LPO clock or 1000 Hz for RTC clock */
    return configLPTMR_CLOCK_HZ;
}

/**
 * @brief Calculate elapsed ticks based on current LPTMR timer state
 *
 * This function calculates how many tick periods have elapsed based on the current timer state.
 * For LPTMR, we use the current timer count directly since it's a free-running counter.
 *
 * @param base Base address of the LPTMR peripheral
 * @param reload_value The reload value that was set for the timer
 * @param expected_idle_time The expected idle time in ticks
 * @return Number of tick periods that have elapsed
 */
static uint32_t lptmr_calculate_elapsed_ticks(void *base, uint32_t reload_value, uint32_t expected_idle_time)
{
    LPTMR_Type *lptmrBase  = (LPTMR_Type *)base;
    uint32_t current_count = LPTMR_GetCurrentTimerCount(lptmrBase);

    /* Calculate timer counts for one tick */
    uint32_t timer_counts_for_one_tick = lptmr_get_timer_frequency() / configTICK_RATE_HZ;

    /* Calculate complete tick periods */
    uint32_t complete_tick_periods = current_count / timer_counts_for_one_tick;

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

/**
 * @brief LPTMR HAL implementation
 *
 * This structure provides the LPTMR implementation of the tickless_timer_hal_t interface.
 */
static const tickless_timer_hal_t lptmr_hal = {.init                    = lptmr_init,
                                               .get_timer_base          = lptmr_get_timer_base,
                                               .get_timer_irqn          = lptmr_get_timer_irqn,
                                               .start_timer             = lptmr_start_timer,
                                               .stop_timer              = lptmr_stop_timer,
                                               .get_current_count       = lptmr_get_current_count,
                                               .set_timer_period        = lptmr_set_timer_period,
                                               .clear_status_flags      = lptmr_clear_status_flags,
                                               .isr_handler             = lptmr_isr_handler,
                                               .get_timer_frequency     = lptmr_get_timer_frequency,
                                               .calculate_elapsed_ticks = lptmr_calculate_elapsed_ticks};

/**
 * @brief Get the LPTMR timer HAL implementation
 *
 * This function returns a pointer to the LPTMR implementation of the tickless_timer_hal_t interface.
 * It is conditionally compiled based on the CONFIG_TICKLESS_TIMER_LPTMR configuration option.
 *
 * @return Pointer to the LPTMR timer HAL implementation
 */
#if defined(CONFIG_TICKLESS_TIMER_LPTMR)
const tickless_timer_hal_t *tickless_get_timer_hal(void)
{
    return &lptmr_hal;
}
#endif

#endif /* configUSE_TICKLESS_IDLE == 2 */
