/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_clock.h"
#include "timer.h"

#include <stdint.h>
#include <sys/time.h>
#include <xtensa/xos.h>

/*******************************************************************************
 * Code
 ******************************************************************************/

void TIMER_Init()
{
#if (defined(MIMXRT798S_hifi4_SERIES))
	xos_set_clock_freq(CLOCK_GetFreq(kCLOCK_Hifi4CpuClk));
#elif  (defined(MIMXRT798S_hifi1_SERIES))
	xos_set_clock_freq(CLOCK_GetFreq(kCLOCK_Hifi1CpuClk));
#elif (defined(MIMXRT595S_dsp_SERIES) || defined(MIMXRT685S_dsp_SERIES))
	xos_set_clock_freq(CLOCK_GetFreq(kCLOCK_DspCpuClk));
#endif
    xos_start_system_timer(-1, 0);
}

int TIMER_GetTimeInUS()
{
    return xos_get_system_cycles() / (xos_get_clock_freq() / 1000000);
}

/* Override the default Xtensa newlib gettimeofday(), which converts CCOUNT
 * using a compile-time 2^24 Hz assumption and mis-scales time by ~18x on
 * parts where the DSP runs at a few hundred MHz. Use the XOS system timer
 * instead (clocked at the CPU frequency set by TIMER_Init). This also fixes
 * std::chrono::steady_clock and the ExecuTorch event tracer timestamps,
 * both derived from gettimeofday(). */
int gettimeofday(struct timeval *__restrict tv, void *__restrict tz)
{
    (void)tz;
    uint64_t us = xos_get_system_cycles() / (xos_get_clock_freq() / 1000000U);
    tv->tv_sec  = (time_t)(us / 1000000U);
    tv->tv_usec = (suseconds_t)(us % 1000000U);
    return 0;
}
