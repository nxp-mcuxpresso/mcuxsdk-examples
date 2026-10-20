/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "perf_log.h"

#include <assert.h>

/* ---- Print hook ---- */
#ifndef PERF_LOG_PRINTF
#include "fsl_debug_console.h"
#define PERF_LOG_PRINTF PRINTF
#endif

#ifndef PERF_LOG_ASSERT
#define PERF_LOG_ASSERT(expr) assert(expr)
#endif

extern uint32_t SystemCoreClock;

/* ---- Module state ---- */

static perf_log_record_t s_records[PERF_LOG_MAX_RECORDS];
static uint32_t          s_count;
static uint32_t          s_dropped;

/* Core clock latched by perf_log_init(), used for all conversions so that a
 * later clock change does not corrupt already collected measurements. */
static uint32_t s_core_clock;

#if defined(PERF_LOG_BACKEND_DWT)

/* ---- DWT backend (devices implementing the DWT cycle counter) ---- */

/* Armv8-M renamed the debug control block from CoreDebug to DCB. Newer CMSIS
 * versions keep CoreDebug only as a deprecated alias, so select whichever the
 * device header actually provides. */
#if defined(DCB_DEMCR_TRCENA_Msk)
#define PERF_LOG_DEMCR       (DCB->DEMCR)
#define PERF_LOG_DEMCR_TRCENA DCB_DEMCR_TRCENA_Msk
#else
#define PERF_LOG_DEMCR       (CoreDebug->DEMCR)
#define PERF_LOG_DEMCR_TRCENA CoreDebug_DEMCR_TRCENA_Msk
#endif

int perf_log_is_available(void)
{
#ifdef DWT_CTRL_NOCYCCNT_Msk
    return ((DWT->CTRL & DWT_CTRL_NOCYCCNT_Msk) == 0U);
#else
    return 1;
#endif
}

void perf_log_init(void)
{
    perf_log_reset();

    s_core_clock = SystemCoreClock;

    PERF_LOG_DEMCR |= PERF_LOG_DEMCR_TRCENA;

    PERF_LOG_ASSERT(perf_log_is_available());
    if (!perf_log_is_available())
    {
        return;
    }

    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t perf_log_get_cycles(void)
{
    return perf_log_is_available() ? DWT->CYCCNT : 0U;
}

void perf_log_tick(void)
{
    /* The DWT counter is a true 32-bit counter, nothing to service. */
}

void perf_log_systick_irq_handler(void)
{
    /* Not used by the DWT backend, provided so that an application call site
     * does not have to be conditional. */
}

#else /* PERF_LOG_BACKEND_SYSTICK */

/* ---- SysTick backend (devices without a DWT cycle counter) ----
 *
 * SysTick is a 24-bit down counter. It is programmed to free-run over the
 * full range and the elapsed periods are counted in software, which extends
 * it to a synthetic 32-bit up counter. The returned value is up-counting so
 * that the elapsed time is plain unsigned subtraction, exactly as with the
 * DWT backend.
 *
 * Wraps are counted either by the SysTick exception, which cannot miss one,
 * or by polling COUNTFLAG. COUNTFLAG is set by hardware on every wrap and
 * cleared by reading SysTick->CTRL, so a polling read only recovers a wrap
 * that happened since the previous read and never more than one.
 */

#define PERF_LOG_SYSTICK_PERIOD (PERF_LOG_SYSTICK_RELOAD + 1U)

/* Number of elapsed SysTick periods. */
static volatile uint32_t s_systick_wraps;
static int               s_systick_started;

int perf_log_is_available(void)
{
    return s_systick_started;
}

void perf_log_init(void)
{
    perf_log_reset();

    s_core_clock = SystemCoreClock;

    /* Stop the counter before reprogramming it so that no stale COUNTFLAG or
     * partial reload is observed by the first measurement. */
    SysTick->CTRL = 0U;
    SysTick->LOAD = PERF_LOG_SYSTICK_RELOAD;
    SysTick->VAL  = 0U;

    s_systick_wraps   = 0U;
    s_systick_started = 1;

    /* Discard the COUNTFLAG left over from the previous configuration. */
    (void)SysTick->CTRL;

#if (PERF_LOG_SYSTICK_IRQ == 1)
    /* Lowest possible priority: the handler only increments a counter and
     * must not add latency to the application. */
    NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
#else
    /* Processor clock, counter enabled, exception disabled. */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
#endif
}

void perf_log_systick_irq_handler(void)
{
    s_systick_wraps++;
}

#if (PERF_LOG_SYSTICK_IRQ == 1) && (PERF_LOG_SYSTICK_PROVIDE_HANDLER == 1)
void SysTick_Handler(void)
{
    perf_log_systick_irq_handler();
}
#endif

uint32_t perf_log_get_cycles(void)
{
    uint32_t val;
    uint32_t wraps;

    if (!s_systick_started)
    {
        return 0U;
    }

#if (PERF_LOG_SYSTICK_IRQ == 1)
    /*
     * The wrap counter and VAL are separate reads, so a wrap between them
     * would pair a high part with a low part from a different period. Re-read
     * until both come from the same period: VAL is a down counter, so a wrap
     * between the two VAL reads shows up as the second read being larger.
     */
    do
    {
        wraps = s_systick_wraps;
        val   = SysTick->VAL;
    } while ((wraps != s_systick_wraps) || (val < SysTick->VAL));
#else
    /* Read VAL, then CTRL, then VAL again. If the counter wrapped between the
     * two VAL reads the first one belongs to the previous period, so the
     * second read is paired with the already consumed COUNTFLAG. */
    val = SysTick->VAL;

    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
        s_systick_wraps++;
        val = SysTick->VAL;
    }

    wraps = s_systick_wraps;
#endif

    /* Convert the down count into an up count within the current period. */
    return (wraps * PERF_LOG_SYSTICK_PERIOD) + (PERF_LOG_SYSTICK_RELOAD - val);
}

void perf_log_tick(void)
{
#if (PERF_LOG_SYSTICK_IRQ == 1)
    /* Wraps are counted by the exception, nothing to service. */
#else
    (void)perf_log_get_cycles();
#endif
}

#endif /* backend */

uint32_t perf_log_max_interval(void)
{
    return PERF_LOG_MAX_INTERVAL;
}

/* Clock used for conversions, falls back to the current one if perf_log_init()
 * was never called. */
static uint32_t perf_log_clock(void)
{
    if (s_core_clock == 0U)
    {
        s_core_clock = SystemCoreClock;
    }

    return s_core_clock;
}

uint32_t perf_log_cycles_to_us(uint32_t cycles)
{
    uint32_t clock = perf_log_clock();

    if (clock == 0U)
    {
        return 0U;
    }

    return (uint32_t)(((uint64_t)cycles * 1000000ULL) / clock);
}

uint32_t perf_log_cycles_to_ms(uint32_t cycles)
{
    uint32_t clock = perf_log_clock();

    if (clock == 0U)
    {
        return 0U;
    }

    return (uint32_t)(((uint64_t)cycles * 1000ULL) / clock);
}

/* ---- Record table ---- */

void perf_log_add(const char *label, uint32_t cycles)
{
    if (s_count < (uint32_t)PERF_LOG_MAX_RECORDS)
    {
        s_records[s_count].label  = label;
        s_records[s_count].cycles = cycles;
        s_count++;
    }
    else
    {
        s_dropped++;
    }
}

void perf_log_reset(void)
{
    s_count   = 0U;
    s_dropped = 0U;
}

void perf_log_report(void)
{
    uint32_t i;
    uint32_t clock    = perf_log_clock();
    uint32_t range_ms = perf_log_cycles_to_ms(PERF_LOG_MAX_INTERVAL);

#if defined(PERF_LOG_BACKEND_DWT)
    static const char *const backend = "DWT";
#else
    static const char *const backend = "SysTick";
#endif

    PERF_LOG_PRINTF("\n"
                    "================================\n"
                    " BOOT PERFORMANCE REPORT\n"
                    " Timer backend: %12s\n"
                    " Core clock: %15u MHz\n",
                    backend, (unsigned)clock / 1000000);

    /* A short counter period on a fast core can round down to 0 ms, so fall
     * back to microseconds to keep the printed limit meaningful. */
    if (range_ms != 0U)
    {
        PERF_LOG_PRINTF(" Max measurable range: %5u ms\n\n", (unsigned)range_ms);
    }
    else
    {
        PERF_LOG_PRINTF(" Max measurable range: %5u us\n\n",
                        (unsigned)perf_log_cycles_to_us(PERF_LOG_MAX_INTERVAL));
    }

    for (i = 0U; i < s_count; i++)
    {
        uint32_t    cycles = s_records[i].cycles;
        uint32_t    us     = perf_log_cycles_to_us(cycles);
        const char *suspect = "";

        /* A record at or beyond the counter range has folded back and its
         * value is meaningless, so mark it instead of printing it as a
         * trustworthy number. */
        if (cycles >= PERF_LOG_MAX_INTERVAL)
        {
            suspect = " (overflow)";
        }

        /* Short intervals are printed in us so that the ms truncation does
         * not hide them, longer ones in ms for readability. */
        if (us < (PERF_LOG_US_THRESHOLD_MS * 1000U))
        {
            PERF_LOG_PRINTF(" %-19s %7u us%s\n", s_records[i].label, (unsigned)us, suspect);
        }
        else
        {
            PERF_LOG_PRINTF(" %-19s %7u ms%s\n", s_records[i].label, (unsigned)perf_log_cycles_to_ms(cycles),
                            suspect);
        }
    }

    if (s_dropped != 0U)
    {
        PERF_LOG_PRINTF("[PERF] %u measurement(s) dropped, increase PERF_LOG_MAX_RECORDS\n",
                        (unsigned)s_dropped);
    }
    PERF_LOG_PRINTF("================================\n\n");
}

const perf_log_record_t *perf_log_get_records(void)
{
    return s_records;
}

uint32_t perf_log_get_count(void)
{
    return s_count;
}

uint32_t perf_log_get_dropped(void)
{
    return s_dropped;
}
