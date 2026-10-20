/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PERF_LOG_H__
#define __PERF_LOG_H__

#include <stdint.h>


/*
 * Lightweight time measurement log based on a free-running core clock
 * counter.
 *
 * Measurements are not printed immediately. Every PERF_LOG_END() stores one
 * record (compile-time label string + elapsed cycle count) into a static
 * table. Call PERF_LOG_REPORT() to print the whole table when needed.
 *
 * Raw cycles are stored to keep full counter resolution.
 *
 * Two hardware backends are supported:
 *
 *   PERF_LOG_BACKEND_DWT     - DWT CYCCNT, a true 32-bit free-running cycle
 *                              counter. Used when the device implements the
 *                              DWT cycle counter.
 *   PERF_LOG_BACKEND_SYSTICK - SysTick used as a 24-bit free-running down
 *                              counter with software wrap tracking. Used on
 *                              devices without a DWT cycle counter (for
 *                              example Cortex-M0, M0+, and the Armv8-M
 *                              Baseline parts that omit the DWT).
 *
 * The backend is auto-selected from the CMSIS register definitions provided
 * by the device header: the DWT backend is chosen only when the CYCCNT
 * control bit is actually defined, because the DWT is optional on Armv7-M
 * and Armv8-M and many devices leave it out. Define PERF_LOG_BACKEND_DWT or
 * PERF_LOG_BACKEND_SYSTICK before including this header to force one of
 * them.
 *
 * CMSIS core header is auto-selected from __CORTEX_M when available,
 * otherwise from unambiguous compiler architecture macros.
 * To override, define PERF_LOG_CORE_HEADER.
 *
 * PERF_LOG_SILENT: define to disable recording and reporting completely.
 * Measurements still run and PERF_LOG_END still returns the elapsed cycles.
 *
 * SysTick backend note:
 *   perf_log_init() takes over SysTick: it is reprogrammed to the processor
 *   clock with reload 0x00FFFFFF. Do not use the SysTick backend when
 *   SysTick drives an RTOS tick or a delay function.
 *
 *   Because SysTick is only 24 bits wide, the counter wraps every 2^24 core
 *   clock cycles (~233 ms at 72 MHz). Wraps are counted in software, and how
 *   they are detected decides the usable measurement range:
 *
 *   PERF_LOG_SYSTICK_IRQ = 1 (default)
 *     The SysTick exception is enabled and every wrap is counted by the
 *     handler, so wraps that happen inside the measured code are not lost
 *     and the range is the full 2^32 cycles, as with the DWT backend.
 *     Measured code must therefore run with interrupts enabled and must not
 *     mask them for a whole counter period.
 *
 *     By default this module defines SysTick_Handler. If the application or
 *     an RTOS already owns that vector, build with
 *     PERF_LOG_SYSTICK_PROVIDE_HANDLER = 0 and call
 *     perf_log_systick_irq_handler() from the existing handler.
 *
 *   PERF_LOG_SYSTICK_IRQ = 0
 *     Polling only: a wrap is detected when a counter read observes it, so
 *     at most one wrap can be recovered per read and a single measured
 *     interval must stay below 2^24 cycles, unless perf_log_tick() is called
 *     often enough to absorb the wraps.
 *
 * Overflow note:
 *   A single measured interval must be shorter than the measurable range of
 *   the active backend (for the DWT backend 2^32 core clock cycles, for
 *   example ~7.2 s at 600 MHz, ~28.6 s at 150 MHz).
 *
 *   Unsigned arithmetic handles the allowed wrap correctly, but longer
 *   intervals silently fold back into the measurable range. The range is
 *   printed by perf_log_report() so the limit is always visible, and any
 *   record that reaches it is flagged as unreliable.
 */

#include "fsl_common.h"
#if defined(PERF_LOG_CORE_HEADER)
  /* User manual override */
  #include PERF_LOG_CORE_HEADER
#elif defined(__CORTEX_M) && (__CORTEX_M == 85U)
  #include "core_cm85.h"
#elif defined(__CORTEX_M) && (__CORTEX_M == 55U)
  #include "core_cm55.h"
#elif defined(__CORTEX_M) && (__CORTEX_M == 33U)
  #include "core_cm33.h"
#elif defined(__CORTEX_M) && (__CORTEX_M == 7U)
  #include "core_cm7.h"
#elif defined(__CORTEX_M) && (__CORTEX_M == 4U)
  #include "core_cm4.h"
#elif defined(__CORTEX_M) && (__CORTEX_M == 3U)
  #include "core_cm3.h"
#elif defined(__ARM_ARCH_8_1M_MAIN__)
  #error "Perf Log: __ARM_ARCH_8_1M_MAIN__ is ambiguous between Cortex-M55 and Cortex-M85. Include your device header first or define PERF_LOG_CORE_HEADER manually."
#elif defined(__ARM_ARCH_8M_MAIN__)
  #include "core_cm33.h"
#elif defined(__ARM_ARCH_7EM__)
  #error "Perf Log: __ARM_ARCH_7EM__ is ambiguous between Cortex-M4 and Cortex-M7. Include your device header first or define PERF_LOG_CORE_HEADER manually."
#elif defined(__ARM_ARCH_7M__)
  #include "core_cm3.h"
#elif defined(__CORTEX_M) && (__CORTEX_M == 23U)
  #include "core_cm23.h"
#elif defined(__CORTEX_M) && (__CORTEX_M == 0U)
  #ifdef __CORTEX_SC
    #include "core_cm0plus.h"
  #else
    #include "core_cm0.h"
  #endif
#elif defined(__ARM_ARCH_8M_BASE__)
  #include "core_cm23.h"
#elif defined(__ARM_ARCH_6M__)
  #include "core_cm0plus.h"
#else
  #error "Perf Log: unable to detect ARM Cortex-M core. Define PERF_LOG_CORE_HEADER manually."
#endif

/* ---- Backend selection ---- */

/*
 * The DWT is an optional unit, so presence of the DWT_BASE address alone does
 * not mean the cycle counter exists. Require the CYCCNT enable bit and a
 * debug control block to unlock tracing, both of which the CMSIS core header
 * only defines when the unit is actually implemented. On Armv8-M the debug
 * control block is named DCB instead of CoreDebug.
 */
#if defined(CoreDebug_DEMCR_TRCENA_Msk) || defined(DCB_DEMCR_TRCENA_Msk)
  #define PERF_LOG_HAVE_DEMCR 1
#else
  #define PERF_LOG_HAVE_DEMCR 0
#endif

#if defined(DWT_CTRL_CYCCNTENA_Msk) && (PERF_LOG_HAVE_DEMCR == 1)
  #define PERF_LOG_HAVE_CYCCNT 1
#else
  #define PERF_LOG_HAVE_CYCCNT 0
#endif

#if defined(PERF_LOG_BACKEND_DWT) && defined(PERF_LOG_BACKEND_SYSTICK)
  #error "Perf Log: define only one of PERF_LOG_BACKEND_DWT / PERF_LOG_BACKEND_SYSTICK."
#elif defined(PERF_LOG_BACKEND_DWT) && (PERF_LOG_HAVE_CYCCNT == 0)
  #error "Perf Log: PERF_LOG_BACKEND_DWT requested but this device does not define the DWT cycle counter registers. Use PERF_LOG_BACKEND_SYSTICK."
#elif !defined(PERF_LOG_BACKEND_DWT) && !defined(PERF_LOG_BACKEND_SYSTICK)
  /* Devices without a DWT cycle counter fall back to SysTick. */
  #if (PERF_LOG_HAVE_CYCCNT == 1)
    #define PERF_LOG_BACKEND_DWT
  #else
    #define PERF_LOG_BACKEND_SYSTICK
  #endif
#endif

#if defined(PERF_LOG_BACKEND_SYSTICK)

/* Count SysTick wraps in the SysTick exception instead of polling for them. */
#ifndef PERF_LOG_SYSTICK_IRQ
#define PERF_LOG_SYSTICK_IRQ 1
#endif

/* Let this module define SysTick_Handler. */
#ifndef PERF_LOG_SYSTICK_PROVIDE_HANDLER
#define PERF_LOG_SYSTICK_PROVIDE_HANDLER 1
#endif

#define PERF_LOG_SYSTICK_RELOAD 0x00FFFFFFU

/*
 * Longest reliably measurable interval, in core clock cycles. With interrupt
 * driven wrap counting no wrap can be missed, so the limit is the width of
 * the returned cycle count. When polling, only the wrap observed by the
 * closing read can be recovered.
 */
#if (PERF_LOG_SYSTICK_IRQ == 1)
#define PERF_LOG_MAX_INTERVAL 0xFFFFFFFFU
#else
#define PERF_LOG_MAX_INTERVAL PERF_LOG_SYSTICK_RELOAD
#endif

#else /* PERF_LOG_BACKEND_DWT */

#define PERF_LOG_MAX_INTERVAL 0xFFFFFFFFU

#endif

#ifndef PERF_LOG_MAX_RECORDS
#define PERF_LOG_MAX_RECORDS 32
#endif

/* Records shorter than this many milliseconds are reported in microseconds. */
#ifndef PERF_LOG_US_THRESHOLD_MS
#define PERF_LOG_US_THRESHOLD_MS 10U
#endif

/* ---- Measurement record ---- */

typedef struct
{
    const char *label;  /* compile-time label string, stored as pointer only */
    uint32_t    cycles; /* elapsed core clock cycles */
} perf_log_record_t;

/* Return non-zero when the active backend counter is usable. */
int perf_log_is_available(void);

/*
 * Start the cycle counter of the active backend, clear the record table and
 * latch the current SystemCoreClock value used for all cycles-to-time
 * conversions. With the SysTick backend this reprograms SysTick.
 */
void perf_log_init(void);

/* Raw 32-bit cycle count, 0 when the counter is not available. */
uint32_t perf_log_get_cycles(void);

/*
 * Service the software wrap tracking of the polling SysTick backend. Call it
 * at least once per 2^24 core clock cycles to measure intervals longer than
 * one SysTick period. No-op with the DWT backend and with interrupt driven
 * wrap counting.
 */
void perf_log_tick(void);

/*
 * SysTick wrap counter, to be called from the application SysTick_Handler
 * when PERF_LOG_SYSTICK_PROVIDE_HANDLER is 0. Always defined so that the
 * call site does not need to be conditional.
 */
void perf_log_systick_irq_handler(void);

/* Longest reliably measurable interval, in core clock cycles. */
uint32_t perf_log_max_interval(void);

/* Convert cycles using the clock latched by perf_log_init(). */
uint32_t perf_log_cycles_to_us(uint32_t cycles);
uint32_t perf_log_cycles_to_ms(uint32_t cycles);

/* Append one measurement. Records beyond the capacity are counted as dropped. */
void perf_log_add(const char *label, uint32_t cycles);

/* Drop all collected measurements. */
void perf_log_reset(void);

/* Print the core clock, the measurable range and the whole record table. */
void perf_log_report(void);

/* Read-only access to the collected records. */
const perf_log_record_t *perf_log_get_records(void);
uint32_t                 perf_log_get_count(void);
uint32_t                 perf_log_get_dropped(void);

/* ---- Macros ---- */

/*
 * Usage:
 *   perf_log_init();
 *   PERF_LOG_START(boot_time);
 *   ... code to measure ...
 *   PERF_LOG_END(boot_time);
 *   ...
 *   PERF_LOG_REPORT();   // print all collected measurements
 *
 * Sequential measurements use unique labels:
 *   PERF_LOG_START(hash);    foo();   PERF_LOG_END(hash);
 *   PERF_LOG_START(verify);  bar();   PERF_LOG_END(verify);
 *
 * Nested measurements - wrap inner pair in a block:
 *   PERF_LOG_START(outer);
 *   outer_code();
 *   { PERF_LOG_START(inner); inner_code(); PERF_LOG_END(inner); }
 *   PERF_LOG_END(outer);
 *
 * PERF_LOG_END returns the elapsed cycle count:
 *   uint32_t cycles = PERF_LOG_END(boot_time);
 *   uint32_t us     = perf_log_cycles_to_us(cycles);
 *
 * To store a measurement under a name different from the C label use
 * PERF_LOG_END_NAMED(label, "my custom name").
 */

#define _PERF_LOG_CONCAT_(a, b) a##b
#define _PERF_LOG_CONCAT(a, b)  _PERF_LOG_CONCAT_(a, b)
#define _PERF_LOG_STR_(s)       #s
#define _PERF_LOG_STR(s)        _PERF_LOG_STR_(s)

#ifdef PERF_LOG_SILENT

#define PERF_LOG_END_NAMED(label, name) ((void)0)
#define PERF_LOG_START(label)           ((void)0)
#define PERF_LOG_END(label)             ((void)0)
#define PERF_LOG_INIT()                 ((void)0)
#define PERF_LOG_REPORT()               ((void)0)
#define PERF_LOG_RESET()                ((void)0)

#else

#define PERF_LOG_END_NAMED(label, name)                                   \
    ({                                                                    \
        uint32_t _perf_log_dt_ =                                          \
            perf_log_get_cycles() - _PERF_LOG_CONCAT(_perf_log_t0_, label);\
        perf_log_add((name), _perf_log_dt_);                              \
        _perf_log_dt_;                                                    \
    })
#define PERF_LOG_START(label) \
    uint32_t _PERF_LOG_CONCAT(_perf_log_t0_, label) = perf_log_get_cycles()
#define PERF_LOG_END(label) PERF_LOG_END_NAMED(label, _PERF_LOG_STR(label))
#define PERF_LOG_INIT()   perf_log_init()
#define PERF_LOG_REPORT() perf_log_report()
#define PERF_LOG_RESET()  perf_log_reset()

#endif



#endif /* __PERF_LOG_H__ */
