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
extern void xthal_dcache_region_writeback_inv(void *addr, size_t size);

void cleanCache(void)
{
    //xthal_dcache_region_writeback((void *)0x204e0000, 0xaf00);
    xthal_dcache_region_writeback_inv((void *)0x204e0000, 0xaf00);
}

void invalidateCache(void)
{
    xthal_dcache_region_invalidate((void *)0x204e0000, 0xaf00);
}

void BOARD_Init(void)
{
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* Note: need tell clock driver the frequency of OSC. */

    BOARD_InitBootPins();
    BOARD_InitDebugConsole();

    PRINTF("DSP Init Susccessfully\r\n");
}
/*${function:end}*/
