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

extern void xthal_dcache_all_writeback();
extern void xthal_dcache_all_invalidate();
extern void xthal_icache_all_invalidate();

void cleanCache(void)

{
    xthal_dcache_all_writeback();
}

void invalidateCache(void)

{
    xthal_dcache_all_invalidate();
    xthal_icache_all_invalidate();
}

void BOARD_Init(void)
{

    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* Note: need tell clock driver the frequency of OSC. */

    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    PRINTF("DSP Init Susccessfully\r\n");
}
/*${function:end}*/
