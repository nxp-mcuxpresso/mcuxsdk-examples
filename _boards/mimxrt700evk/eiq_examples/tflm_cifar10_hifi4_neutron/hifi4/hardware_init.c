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
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/
/*${function:start}*/
#include "fsl_debug_console.h"

extern void xthal_dcache_region_invalidate(void *addr, size_t size);
extern void xthal_dcache_region_writeback(void *addr, size_t size);

// Provide strong definitions to override the Neutron firmware library's weak
// default cache hooks, so cache maintenance is done by exact address range.
void cleanCacheByRange(uint32_t address, uint32_t size_byte)
{
    xthal_dcache_region_writeback((void *)address, size_byte);
}

void invalidateCacheByRange(uint32_t address, uint32_t size_byte)
{
    xthal_dcache_region_invalidate((void *)address, size_byte);
}

void BOARD_Init(void)
{
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* Note: need tell clock driver the frequency of OSC. */

    BOARD_InitBootPins();
    BOARD_InitDebugConsole();

    PRINTF("DSP Init Susccessfully\r\n");
}
/*${function:end}*/
