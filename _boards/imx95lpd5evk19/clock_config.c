/*
 * Copyright 2023 NXP
 * All rights reserved.
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
 * Variables
 ******************************************************************************/
/* clang-format off */
/*
 * SYSTEM_PLL1
 *
 * VCO = (24MHz / rdiv) * (mfi + mfn / mfd)  = 4000MHz
 * Output = VCO / odiv = 1000MHz
 */
const fracn_pll_init_t g_sysPllCfg = {
    .rdiv = 1,
    .mfi = 166,
    .mfn = 2,
    .mfd = 3,
    .odiv = 4
};

/* SYSTEM_PLL1_PFD0 Output = VCO / (mfi + mfn / 5) = 1000MHz */
const fracn_pll_pfd_init_t g_sysPllPfd0Cfg = {
    .mfi = 4,
    .mfn = 0,
    .div2_en = true
};

/*SYSTEM_PLL1_PFD1 Output = VCO / (mfi + mfn / 5) = 800MHz */
const fracn_pll_pfd_init_t g_sysPllPfd1Cfg = {
    .mfi = 5,
    .mfn = 0,
    .div2_en = true
};

/* SYSTEM_PLL1_PFD2 Output = VCO / (mfi + mfn / 5) = 625MHz */
const fracn_pll_pfd_init_t g_sysPllPfd2Cfg = {
    .mfi = 6,
    .mfn = 2,
    .div2_en = true
};


/*
 * AUDIOPLL1/AUDIOPLL1OUT
 *
 * VCO = (24MHz / rdiv) * (mfi + (mfn / mfd))  = 3,932,160,000 Hz
 * Output = VCO / odiv = 393.216 MHz
 */
const fracn_pll_init_t g_audioPllCfg = {
    .rdiv = 1,
    .mfi = 163,
    .mfn = 84,
    .mfd = 100,
    .odiv = 10
};


/* clang-format on */

/*******************************************************************************
 * Code
 ******************************************************************************/

static void BOARD_InitClock(void)
{
    g_clockSourceFreq[kCLOCK_Ext]             = 133000000U;
    g_clockSourceFreq[kCLOCK_Osc32K]          = 3000U;
    g_clockSourceFreq[kCLOCK_Osc24M]          = 24000000U;
    g_clockSourceFreq[kCLOCK_Fro]             = 333330000U;
    g_clockSourceFreq[kCLOCK_SysPll1Dfs0]     = 1000000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Dfs0Div2] = 500000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Dfs1]     = 800000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Dfs1Div2] = 400000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Dfs2]     = 666670000U;
    g_clockSourceFreq[kCLOCK_SysPll1Dfs2Div2] = 333330000U;
    g_clockSourceFreq[kCLOCK_AudioPll1]       = 400000000U;
    g_clockSourceFreq[kCLOCK_AudioPll2]       = 400000000U;
    g_clockSourceFreq[kCLOCK_DramPll]         = 1000000000U;
    g_clockSourceFreq[kCLOCK_VideoPll1]       = 700000000U;
    g_clockSourceFreq[kCLOCK_VideoPll2]       = 700000000U;
    g_clockSourceFreq[kCLOCK_VideoPll3]       = 700000000U;
}

void BOARD_BootClockRUN(void)
{
    BOARD_InitClock();

#if 0
    /* ROM has already initialized PLL */
    CLOCK_PllInit(AUDIOPLL, &g_audioPllCfg);
    CLOCK_PllInit(SYSPLL, &g_sysPllCfg);
    CLOCK_PllPfdInit(SYSPLL, 0, &g_sysPllPfd0Cfg);
    CLOCK_PllPfdInit(SYSPLL, 1, &g_sysPllPfd1Cfg);
    CLOCK_PllPfdInit(SYSPLL, 2, &g_sysPllPfd2Cfg);
#endif
}
