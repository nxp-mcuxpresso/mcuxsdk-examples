/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2023, 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_smc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(SCG_SPLLCFG_SOURCE)
#define SCG_SPLL_HAS_CLK_SRC 1
#else
#define SCG_SPLL_HAS_CLK_SRC 0
#endif

/*
 * SIRC clock setting.
 * SIRC clock           : 8MHz
 * SIRCDIV1_CLK output  : 8MHz
 * SIRCDIV2_CLK output  : 4MHz
 * SIRCDIV3_CLK output  : 4MHz
 */
const scg_sirc_config_t s_scgSircConfig = {.enableMode = kSCG_SircEnable | kSCG_SircEnableInLowPower,
#if (defined(FSL_FEATURE_SCG_HAS_SIRCDIV1) && FSL_FEATURE_SCG_HAS_SIRCDIV1)
                                           .div1 = kSCG_AsyncClkDivBy1,
#endif
                                           .div2 = kSCG_AsyncClkDivBy2,
#if (defined(FSL_FEATURE_SCG_HAS_SIRCDIV3) && FSL_FEATURE_SCG_HAS_SIRCDIV3)
                                           .div3 = kSCG_AsyncClkDivBy2,
#endif
                                           .range = kSCG_SircRangeHigh};

/*
 * FIRC clock setting.
 * FIRC clock           : 48MHz
 * FIRCDIV1_CLK output  : 48MHz
 * FIRCDIV2_CLK output  : 48MHz
 * FIRCDIV3_CLK output  : 48MHz
 */
const scg_firc_config_t s_scgFircConfig = {.enableMode = kSCG_FircEnable,
#if (defined(FSL_FEATURE_SCG_HAS_FIRCDIV3) && FSL_FEATURE_SCG_HAS_FIRCDIV3)
                                           .div3 = kSCG_AsyncClkDivBy1,
#endif
                                           .div2 = kSCG_AsyncClkDivBy1,
#if (defined(FSL_FEATURE_SCG_HAS_FIRCDIV1) && FSL_FEATURE_SCG_HAS_FIRCDIV1)
                                           .div1 = kSCG_AsyncClkDivBy1,
#endif
                                           .range      = kSCG_FircRange48M,
                                           .trimConfig = NULL};

/*
 * When SPLL is available, and SPLL doesn't have clock source selection, it will
 * use System OSC as clock source. The System OSC should be initialized.
 */
#if (defined(FSL_FEATURE_SCG_HAS_SPLL) && FSL_FEATURE_SCG_HAS_SPLL) && (!SCG_SPLL_HAS_CLK_SRC)
const scg_sosc_config_t g_scgSysOscConfig =
{
    .freq        = APP_SCG_SOSC_FREQ,
    .enableMode  = kSCG_SysOscEnable,
    .monitorMode = kSCG_SysOscMonitorDisable,
    .div2        = APP_SCG_SOSC_DIV2,
    .workMode    = APP_SCG_SOSC_WORK_MODE,
};
#endif

#if (defined(FSL_FEATURE_SCG_HAS_SPLL) && FSL_FEATURE_SCG_HAS_SPLL)
/*
 * SYSPLL clock setting.
 * SYSPLL clock       : 72MHz
 * SYSPLLDIV1 output  : 72MHz
 * SYSPLLDIV2 output  : 36MHz
 * SYSPLLDIV3 output  : 36MHz
 */
const scg_spll_config_t s_scgSysPllConfig = {.enableMode  = kSCG_SysPllEnable,
                                             .monitorMode = kSCG_SysPllMonitorDisable,
#if (defined(FSL_FEATURE_SCG_HAS_SPLLDIV1) && FSL_FEATURE_SCG_HAS_SPLLDIV1)
                                             .div1 = kSCG_AsyncClkDivBy1,
#endif
                                             .div2 = kSCG_AsyncClkDivBy2,
#if (defined(FSL_FEATURE_SCG_HAS_SPLLDIV3) && FSL_FEATURE_SCG_HAS_SPLLDIV3)
                                             .div3 = kSCG_AsyncClkDivBy2,
#endif
/* If SPLL can use FIRC as the clock source, then use it. FIRC is configured to 48M Hz. */
#if SCG_SPLL_HAS_CLK_SRC
                                             .src    = kSCG_SysPllSrcFirc,
                                             .prediv = 5U,
                                             .mult   = 2U
/* If SPLL can't slect FIRC as clock source, when porting this project, the
 * prediv and mult should be set to the proper value based on the source clock
 * frequency.
 *
 * SPLL freq = source clk freq / (prediv + SCG_SPLL_PREDIV_BASE_VALUE) * (mult + SCG_SPLL_MULT_BASE_VALUE) / 2.
 *
 * Rules to follow:
 * 1. Clock after prediv must be in the range of 8 MHz to 32 MHz
 * 2. When the chip supports high speed run (HSRUN) power mode (FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE == 1),
 *    SPLL will be used in HSRUN mode as system clock. The SPLL clock and s_sysClkConfigSysPllInHsrun should
 *    be configured properly to meet clock frequency requirement in HSRUN mode, for example,
 *    the system clock, bus clock, and so on shall be in allowed range.
 *
 */
#else
                                             .prediv = APP_SCG_SPLL_PREDIV,
                                             .mult   = APP_SCG_SPLL_MULT

#endif /* SCG_SPLL_HAS_CLK_SRC */
};
#endif

#if (defined(FSL_FEATURE_SCG_HAS_LPFLL) && FSL_FEATURE_SCG_HAS_LPFLL)
/*
 * LPFLL clock setting.
 * LPFLL clock       : 48MHz
 * LPFLLDIV1 output  : 48MHz
 * LPFLLDIV2 output  : 24MHz
 * LPFLLDIV3 output  : 24MHz
 */
const scg_lpfll_config_t s_scgLpFllConfig = {.enableMode = kSCG_LpFllEnable,
#if (defined(FSL_FEATURE_SCG_HAS_FLLDIV1) && FSL_FEATURE_SCG_HAS_FLLDIV1)
                                             .div1 = kSCG_AsyncClkDivBy1,
#endif
                                             .div2 = kSCG_AsyncClkDivBy2,
#if (defined(FSL_FEATURE_SCG_HAS_FLLDIV3) && FSL_FEATURE_SCG_HAS_FLLDIV3)
                                             .div3 = kSCG_AsyncClkDivBy2,
#endif
                                             .range      = kSCG_LpFllRange48M,
                                             .trimConfig = NULL};
#endif

/*
 * System clock configuration while using SIRC in RUN mode.
 * Core clock : 8MHz
 * Plat clock : 8MHz
 * Bus clock  : 8MHz
 * Slow clock : 4MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSircInRun = {.divSlow = kSCG_SysClkDivBy2,
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
                                                      .divBus = kSCG_SysClkDivBy1,
#endif
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
                                                      .divPlat = kSCG_SysClkDivBy1,
#endif
                                                      .divCore = kSCG_SysClkDivBy1,
                                                      .src     = kSCG_SysClkSrcSirc};

/*
 * System clock configuration while using SIRC in VLPR mode.
 * Core clock : 4MHz
 * Plat clock : 4MHz
 * Bus clock  : 4MHz
 * Slow clock : 1MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSircInVlpr = {.divSlow = kSCG_SysClkDivBy4,
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
                                                       .divBus = kSCG_SysClkDivBy1,
#endif
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
                                                       .divPlat = kSCG_SysClkDivBy1,
#endif
                                                       .divCore = kSCG_SysClkDivBy2,
                                                       .src     = kSCG_SysClkSrcSirc};

/*
 * System clock configuration while using FIRC in RUN mode.
 * Core clock : 48MHz
 * Plat clock : 48MHz
 * Bus clock  : 24MHz
 * Slow clock : 24MHz
 */
const scg_sys_clk_config_t s_sysClkConfigFircInRun = {.divSlow = kSCG_SysClkDivBy2,
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
                                                      .divBus = kSCG_SysClkDivBy2,
#endif
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
                                                      .divPlat = kSCG_SysClkDivBy1,
#endif
                                                      .divCore = kSCG_SysClkDivBy1,
                                                      .src     = kSCG_SysClkSrcFirc};

#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
#if defined(FSL_FEATURE_SCG_HAS_SPLL) && FSL_FEATURE_SCG_HAS_SPLL
/*
 * System clock configuration while using SYSPLL in HSRUN mode.
 * Core clock : 72MHz
 * Plat clock : 72MHz
 * Bus clock  : 36MHz
 * Slow clock : 24MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSysPllInHsrun = {.divSlow = kSCG_SysClkDivBy3,
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
                                                          .divBus = kSCG_SysClkDivBy2,
#endif
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
                                                          .divPlat = kSCG_SysClkDivBy1,
#endif
                                                          .divCore = kSCG_SysClkDivBy1,
                                                          .src     = kSCG_SysClkSrcSysPll};
#endif /* FSL_FEATURE_SCG_HAS_SPLL */

#if defined(FSL_FEATURE_SCG_HAS_LPFLL) && FSL_FEATURE_SCG_HAS_LPFLL
/*
 * System clock configuration while using LPFLL in HSRUN mode.
 * Core clock : 48MHz
 * Plat clock : 48MHz
 * Bus clock  : 24MHz
 * Slow clock : 24MHz
 */
const scg_sys_clk_config_t s_sysClkConfigLpFllInHsrun = {.divSlow = kSCG_SysClkDivBy2,
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
                                                         .divBus = kSCG_SysClkDivBy2,
#endif
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
                                                         .divPlat = kSCG_SysClkDivBy1,
#endif
                                                         .divCore = kSCG_SysClkDivBy1,
                                                         .src     = kSCG_SysClkSrcLpFll};
#endif /* FSL_FEATURE_SCG_HAS_LPFLL */

#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    scg_sys_clk_config_t sysClkConfig;
    scg_sys_clk_config_t curSysClkConfig;

    /*
     * Setup SIRC:
     * On some platforms, SIRC is used by default after reset, while on some
     * other platforms, FIRC is used after reset. So at the beginning, align to
     * use SIRC as system clock source.
     */
    CLOCK_GetCurSysClkConfig(&sysClkConfig);

    if (kSCG_SysClkSrcSirc == sysClkConfig.src)
    {
        CLOCK_InitFirc(&s_scgFircConfig);
        CLOCK_SetRunModeSysClkConfig(&s_sysClkConfigFircInRun);

        /* Wait for clock source change finished. */
        do
        {
            CLOCK_GetCurSysClkConfig(&curSysClkConfig);
        } while (curSysClkConfig.src != s_sysClkConfigFircInRun.src);

        CLOCK_InitSirc(&s_scgSircConfig);
        CLOCK_SetRunModeSysClkConfig(&s_sysClkConfigSircInRun);

        /* Wait for clock source change finished. */
        do
        {
            CLOCK_GetCurSysClkConfig(&curSysClkConfig);
        } while (curSysClkConfig.src != s_sysClkConfigSircInRun.src);

        CLOCK_DeinitFirc();
    }
    else
    {
        CLOCK_InitSirc(&s_scgSircConfig);
        CLOCK_SetRunModeSysClkConfig(&s_sysClkConfigSircInRun);

        /* Wait for clock source change finished. */
        do
        {
            CLOCK_GetCurSysClkConfig(&curSysClkConfig);
        } while (curSysClkConfig.src != s_sysClkConfigSircInRun.src);
    }

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\nSCG Example Start\r\n");

    /* Init FIRC clock. */
    CLOCK_InitFirc(&s_scgFircConfig);

#if (defined(FSL_FEATURE_SCG_HAS_SPLL) && FSL_FEATURE_SCG_HAS_SPLL)

#if !SCG_SPLL_HAS_CLK_SRC
     /* When SPLL doesn't have clock source selection, it uses OSC as clock source,
      * so init OSC before init SPLL.
      */
    CLOCK_InitSysOsc(&g_scgSysOscConfig);
    /* Set the XTAL0 frequency based on board settings. */
    CLOCK_SetXtal0Freq(g_scgSysOscConfig.freq);
#endif

    /*
     * Init SYSPLL clock, SYSPLL uses FIRC as clock source, so must init FIRC
     * before init SYSPLL.
     */
    CLOCK_InitSysPll(&s_scgSysPllConfig);
#endif

#if (defined(FSL_FEATURE_SCG_HAS_LPFLL) && FSL_FEATURE_SCG_HAS_LPFLL)
    /*
     * Init LPFLL clock, LPFLL uses FIRC as clock source, so must init FIRC
     * before init LPFLL.
     */
    CLOCK_InitLpFll(&s_scgLpFllConfig);
#endif

    /* Set system clock configuration for different power modes. */
    CLOCK_SetRunModeSysClkConfig(&s_sysClkConfigFircInRun);

    /* Wait for clock source change finished. */
    do
    {
        CLOCK_GetCurSysClkConfig(&curSysClkConfig);
    } while (curSysClkConfig.src != s_sysClkConfigFircInRun.src);

    CLOCK_SetVlprModeSysClkConfig(&s_sysClkConfigSircInVlpr);
#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
#if defined(FSL_FEATURE_SCG_HAS_SPLL) && FSL_FEATURE_SCG_HAS_SPLL
    CLOCK_SetHsrunModeSysClkConfig(&s_sysClkConfigSysPllInHsrun);
#endif
#if defined(FSL_FEATURE_SCG_HAS_LPFLL) && FSL_FEATURE_SCG_HAS_LPFLL
    CLOCK_SetHsrunModeSysClkConfig(&s_sysClkConfigLpFllInHsrun);
#endif
#endif

    PRINTF("\r\nSCG clock source frequency\r\n");
    PRINTF("SIRC clock: %d Hz\r\n", CLOCK_GetSircFreq());
    PRINTF("FIRC clock: %d Hz\r\n", CLOCK_GetFircFreq());
#if (defined(FSL_FEATURE_SCG_HAS_SPLL) && FSL_FEATURE_SCG_HAS_SPLL)
    PRINTF("SPLL clock: %d Hz\r\n", CLOCK_GetSysPllFreq());
#endif
#if (defined(FSL_FEATURE_SCG_HAS_LPFLL) && FSL_FEATURE_SCG_HAS_LPFLL)
    PRINTF("LPFLL clock: %d Hz\r\n", CLOCK_GetLpFllFreq());
#endif

/*
 * Then system clock configuration is s_sysClkConfigSircInVlpr.
 * Core clock : 4MHz
 * Slow clock : 1MHz
 */
#if (defined(FSL_FEATURE_SMC_HAS_LPWUI) && FSL_FEATURE_SMC_HAS_LPWUI)
    SMC_SetPowerModeVlpr(SMC, false);
#else
    SMC_SetPowerModeVlpr(SMC);
#endif
    while (kSMC_PowerStateVlpr != SMC_GetPowerModeState(SMC))
    {
    }

    PRINTF("\r\nCurrent power mode: VLPR\r\n");
    PRINTF("Core clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkCore));
    PRINTF("Slow clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkSlow));
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
    PRINTF("Bus clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkBus));
#endif
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
    PRINTF("Platform clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkPlat));
#endif

    /*
     * Now system clock configuration is s_sysClkConfigFircInRun.
     * Core clock : 48MHz
     * Slow clock : 24MHz
     */
    SMC_SetPowerModeRun(SMC);
    while (kSMC_PowerStateRun != SMC_GetPowerModeState(SMC))
    {
    }

    /*
     * The system clock configuration does not change until the target clock
     * source is valid. FIRC is disabled in VLPR mode, so after change to RUN mode,
     * it is not selected as system clock source until it is valid. Here wait for
     * FIRC valid.
     */
    while (!CLOCK_IsFircValid())
    {
    }

    PRINTF("\r\nCurrent system clock: RUN\r\n");
    PRINTF("Core clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkCore));
    PRINTF("Slow clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkSlow));
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
    PRINTF("Bus clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkBus));
#endif
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
    PRINTF("Platform clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkPlat));
#endif

#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
#if (defined(FSL_FEATURE_SCG_HAS_SPLL) && FSL_FEATURE_SCG_HAS_SPLL)
    /*
     * Then system clock configuration is s_sysClkConfigSysPllInHsrun.
     * Core clock : 72MHz
     * Slow clock : 24MHz
     */
    /*
     * SYSPLL is diabled in VLPR mode, before change to HSRUN mode, wait it valid.
     */
    while (!CLOCK_IsSysPllValid())
    {
    }
#endif /* FSL_FEATURE_SCG_HAS_SPLL */
#if (defined(FSL_FEATURE_SCG_HAS_LPFLL) && FSL_FEATURE_SCG_HAS_LPFLL)
    /*
     * Then system clock configuration is s_sysClkConfigLpFllInHsrun.
     * Core clock : 72MHz
     * Slow clock : 24MHz
     */
    /*
     * LPFLL is diabled in VLPR mode, before change to HSRUN mode, wait it valid.
     */
    while (!CLOCK_IsLpFllValid())
    {
    }
#endif /* FSL_FEATURE_SCG_HAS_LPFLL */

    SMC_SetPowerModeHsrun(SMC);
    while (kSMC_PowerStateHsrun != SMC_GetPowerModeState(SMC))
    {
    }

    PRINTF("\r\nCurrent power mode: HSRUN\r\n");
    PRINTF("Core clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkCore));
    PRINTF("Slow clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkSlow));
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
    PRINTF("Bus clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkBus));
#endif
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
    PRINTF("Platform clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkPlat));
#endif
#endif

    /*
     * Then system clock configuration is s_sysClkConfigSircInRun
     * Core clock : 8MHz
     * Slow clock : 4MHz
     */
    CLOCK_SetRunModeSysClkConfig(&s_sysClkConfigSircInRun);
    SMC_SetPowerModeRun(SMC);
    while (kSMC_PowerStateRun != SMC_GetPowerModeState(SMC))
    {
    }

    PRINTF("\r\nCurrent power mode: RUN\r\n");
    PRINTF("Core clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkCore));
    PRINTF("Slow clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkSlow));
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
    PRINTF("Bus clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkBus));
#endif
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
    PRINTF("Platform clock: %d Hz\r\n", CLOCK_GetSysClkFreq(kSCG_SysClkPlat));
#endif
    PRINTF("\r\nSCG Example End.\r\n");

    while (1)
    {
    }
}
