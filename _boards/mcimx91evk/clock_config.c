/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "clock_config.h"

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
    BOARD_BootClockRUN();
}

/*******************************************************************************
 * Code
 ******************************************************************************/

static void BOARD_InitClock(void)
{
    g_clockSourceFreq[kCLOCK_Osc24M]          = 24000000U;
    g_clockSourceFreq[kCLOCK_SysPll1]         = 4000000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Pfd0]     = 1000000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Pfd0Div2] = 500000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Pfd1]     = 800000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Pfd1Div2] = 400000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Pfd2]     = 625000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Pfd2Div2] = 312500000U;
    g_clockSourceFreq[kCLOCK_AudioPll1Out]    = 393216000U;
    g_clockSourceFreq[kCLOCK_AudioPll1]       = 393216000U;
}

void BOARD_BootClockRUN(void)
{
    BOARD_InitClock();
}
