/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "timer.h"
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

/*${function:start}*/
#include "fsl_debug_console.h"

#define CORE_DCACHE_LINESIZE 64

extern void xthal_dcache_region_invalidate(void *addr, size_t size);
extern void xthal_dcache_region_writeback(void *addr, size_t size);
extern void xthal_dcache_region_writeback_inv(void *addr, size_t size);

void cleanCacheByRange(uint32_t start_addr, uint32_t size) {
    xthal_dcache_region_writeback((void *)start_addr, size);
}

void invalidateCacheByRange(uint32_t start_addr, uint32_t size) {
    xthal_dcache_region_invalidate((void *)start_addr, size);
}

/* Shared modelrunner code (tf_benchmark.cpp) measures inference time through
 * this abstraction; mirrors the CM33 core implementation. */
int64_t os_clock_now(void) {
    return (int64_t)TIMER_GetTimeInUS();
}

void BOARD_InitHardware(void)
{

    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* Note: need tell clock driver the frequency of OSC. */

    TIMER_Init();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    PRINTF("DSP Init Susccessfully\r\n");
}
/*${function:end}*/
