/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "els_pkc_benchmark_utils.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/** Ms counter */
static volatile uint64_t s_MsCount = 0U;

/** Timeout value for polling */
uint64_t g_Timeout = 0U;

/** Value for poll alarm, either 0 or 1 */
volatile uint8_t g_BenchmarkTimingAlarmed = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
void benchmark_set_alarm(uint64_t seconds)
{
    g_BenchmarkTimingAlarmed = 0U;
    g_Timeout                = benchmark_timing_hardclock() + (seconds * CLOCK_GetCoreSysClkFreq());
}

void benchmark_poll_alarm(void)
{
    if (benchmark_timing_hardclock() > g_Timeout)
    {
        g_BenchmarkTimingAlarmed = 1U;
    }
}

/*!
 * @brief Milliseconds counter since last POR/reset.
 * Function taken from existing mbedtls benchmark.
 */
void SysTick_Handler(void)
{
    s_MsCount++;
}

uint64_t benchmark_timing_hardclock(void)
{
    uint64_t currMsCount;
    uint32_t currTick;
    uint64_t loadTick;

    do
    {
        currMsCount = s_MsCount;
        currTick    = SysTick->VAL;
    } while (currMsCount != s_MsCount);

    loadTick = CLOCK_GetCoreSysClkFreq() / 1000U;
    return (((uint64_t)currMsCount) * loadTick) + loadTick - currTick;
}
