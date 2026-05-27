/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include <xtensa/config/core.h>
#include <xtensa/xos.h>
#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_inputmux.h"
#ifdef MCMGR_USED
#include "mcmgr.h"
#endif
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/
extern void MU4_B_IRQHandler2(void *arg);
/*${function:start}*/
static void XOS_Init(void)
{
    xos_set_clock_freq(XOS_CLOCK_FREQ);
    xos_start_system_timer(-1, 0);
}

void BOARD_InitHardware(void)
{
    /* Disable DSP cache for noncacheable sections. */
    xthal_set_region_attribute((uint32_t *)0x20400000, 30000, XCHAL_CA_BYPASS, 0);
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* Note: need tell clock driver the frequency of OSC. */

    BOARD_InitBootPins();
    BOARD_InitDebugConsole();

    XOS_Init();
}

/*${function:end}*/
