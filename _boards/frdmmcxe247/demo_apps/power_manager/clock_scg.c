/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include <assert.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * In SCG based example, SCG clock sources are configured as:
 *
 * SCG_SOSC: 8MHz.
 * SCG_SIRC: 8MHz.
 * SCG_FIRC: 48MHz.
 * SCG_SPLL: 160MHz.
 *
 * VLPR:
 *  - Clock source: SIRC
 * RUN:
 *  - Clock source: SPLL
 */

/*
 * SIRC clock setting.
 * SIRC clock           : 8MHz
 * SIRCDIV2_CLK output  : 4MHz
 */
const scg_sirc_config_t s_scgSircConfig = {
    .enableMode = kSCG_SircEnable | kSCG_SircEnableInLowPower, /* Enable SIRC clock, Enable SIRC in low power mode */
    .div1 = kSCG_AsyncClkDisable,             /* Slow IRC Clock Divider 1: Clock output is disabled */
    .div2 = kSCG_AsyncClkDivBy2,              /* Slow IRC Clock Divider 2: divided by 2 */
    .range = kSCG_SircRangeHigh,              /* Slow IRC high range clock (8 MHz) */
};

/*
 * FIRC clock setting.
 * FIRC clock           : 48MHz
 * FIRCDIV2_CLK output  : 48MHz
 */
const scg_firc_config_t s_scgFircConfig = {
    .enableMode = kSCG_FircEnable,            /* Enable FIRC clock */
    .div1 = kSCG_AsyncClkDisable,             /* Fast IRC Clock Divider 1: Clock output is disabled */
    .div2 = kSCG_AsyncClkDivBy1,              /* Fast IRC Clock Divider 2: divided by 1 */
    .range = kSCG_FircRange48M,               /* Fast IRC is trimmed to 48MHz */
    .trimConfig = NULL,                       /* Disable trim */
};

/*
 * SOSC clock setting.
 * SOSC clock           : 8MHz
 */
const scg_sosc_config_t s_scgSoscConfig = {
    .freq = 8000000U,                         /* System Oscillator frequency: 8000000Hz */
    .enableMode = kSCG_SysOscEnable,          /* Enable System OSC clock */
    .monitorMode = kSCG_SysOscMonitorDisable, /* Monitor disabled */
    .div1 = kSCG_AsyncClkDisable,             /* System OSC Clock Divider 1: Clock output is disabled */
    .div2 = kSCG_AsyncClkDisable,             /* System OSC Clock Divider 2: Clock output is disabled */
    .workMode = kSCG_SysOscModeOscLowPower,   /* Oscillator low power */
};

/*
 * SPLL clock setting in RUN mode.
 * SPLL clock       : 80MHz
 * SPLLDIV2 output  : 40MHz
 */
const scg_spll_config_t s_scgSysSpllConfigRun = {
    .enableMode = kSCG_SysPllEnable,          /* Enable SPLL clock */
    .monitorMode = kSCG_SysPllMonitorDisable, /* Monitor disabled */
    .div1 = kSCG_AsyncClkDisable,             /* System PLL Clock Divider 1: Clock output is disabled */
    .div2 = kSCG_AsyncClkDivBy4,              /* System PLL Clock Divider 2: divided by 4 */
    .prediv = 0,                              /* Divided by 1 */
    .mult = 24,                               /* Multiply Factor is 40 */
};

/*
 * SPLL clock setting in HSRUN mode.
 * SPLL clock       : 112MHz
 * SPLLDIV2 output  : 56MHz
 */
const scg_spll_config_t s_scgSysSpllConfigHsRun = {
    .enableMode = kSCG_SysPllEnable,          /* Enable SPLL clock */
    .monitorMode = kSCG_SysPllMonitorDisable, /* Monitor disabled */
    .div1 = kSCG_AsyncClkDisable,             /* System PLL Clock Divider 1: Clock output is disabled */
    .div2 = kSCG_AsyncClkDivBy2,              /* System PLL Clock Divider 2: divided by 2 */
    .prediv = 0,                              /* Divided by 1 */
    .mult = 12,                               /* Multiply Factor is 28 */
};

/*
 * System clock configuration while using SIRC in RUN mode.
 * Core clock : 8MHz
 * Slow clock : 4MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSircInRun = {
    .divSlow = kSCG_SysClkDivBy2,             /* Slow Clock Divider: divided by 2 */
    .divBus = kSCG_SysClkDivBy1,              /* Bus Clock Divider: divided by 1 */
    .divCore = kSCG_SysClkDivBy1,             /* Core Clock Divider: divided by 1 */
    .src     = kSCG_SysClkSrcSirc             /* System clock source. */
};
/*
 * System clock configuration while using SIRC in VLPR mode.
 * Core clock : 4MHz
 * Slow clock : 1MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSircInVlpr = {
    .divSlow = kSCG_SysClkDivBy4,             /* Slow Clock Divider: divided by 4 */
    .divBus = kSCG_SysClkDivBy1,              /* Bus Clock Divider: divided by 1 */
    .divCore = kSCG_SysClkDivBy2,             /* Core Clock Divider: divided by 2 */
    .src = kSCG_SysClkSrcSirc,                /* Slow IRC is selected as System Clock Source */
};

/*
 * System clock configuration while using SPLL in RUN mode.
 * Core clock : 80MHz
 * Slow clock : 26.67MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSpllInRun = {
    .divSlow = kSCG_SysClkDivBy3,             /* Slow Clock Divider: divided by 3 */
    .divBus = kSCG_SysClkDivBy2,              /* Bus Clock Divider: divided by 2 */
    .divCore = kSCG_SysClkDivBy2,             /* Core Clock Divider: divided by 2 */
    .src = kSCG_SysClkSrcSysPll,              /* System PLL is selected as System Clock Source */
};

/*
 * System clock configuration while using SPLL in HSRUN mode.
 * Core clock : 112MHz
 * Slow clock : 28MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSpllInHsRun = {
    .divSlow = kSCG_SysClkDivBy4,             /* Slow Clock Divider: divided by 4 */
    .divBus = kSCG_SysClkDivBy2,              /* Bus Clock Divider: divided by 2 */
    .divCore = kSCG_SysClkDivBy1,             /* Core Clock Divider: divided by 1 */
    .src = kSCG_SysClkSrcSysPll,              /* System PLL is selected as System Clock Source */
};

/*
 * Initialize SCG setting after system boot up.
 */
void APP_InitClock(void)
{
    scg_sys_clk_config_t sysClkConfig;
    scg_sys_clk_config_t curSysClkConfig;

    /*
     * Setup SIRC and FIRC:
     * On some platforms, SIRC is used by default after reset, while on some
     * other platforms, FIRC is used after reset. So at the begining, the
     * workflow is different.
     */
    CLOCK_GetCurSysClkConfig(&sysClkConfig);

    CLOCK_InitSirc(&s_scgSircConfig);
    CLOCK_SetRunModeSysClkConfig(&s_sysClkConfigSircInRun);

    /* Wait for clock source change finished. */
    do
    {
        CLOCK_GetCurSysClkConfig(&curSysClkConfig);
    } while (curSysClkConfig.src != s_sysClkConfigSircInRun.src);

    CLOCK_InitFirc(&s_scgFircConfig);

    /* Init SOSC according to board configuration. */
    CLOCK_InitSysOsc(&s_scgSoscConfig);
    /* Set the XTAL0 frequency based on board settings. */
    CLOCK_SetXtal0Freq(s_scgSoscConfig.freq);

    CLOCK_InitSysPll(&s_scgSysSpllConfigRun);

    CLOCK_SetRunModeSysClkConfig(&s_sysClkConfigSpllInRun);

    /* Wait for clock source change finished. */
    do
    {
        CLOCK_GetCurSysClkConfig(&curSysClkConfig);
    } while (curSysClkConfig.src != s_sysClkConfigSpllInRun.src);

    CLOCK_SetVlprModeSysClkConfig(&s_sysClkConfigSircInVlpr);
}

/*
 * Set the clock configuration for RUN mode from VLPR mode.
 */
void APP_SetClockRunFromVlpr(void)
{
    while (!CLOCK_IsSysPllValid())
    {
    }
}

/*
 * Set the clock configuration for VLPR mode.
 */
void APP_SetClockVlpr(void)
{
}

void APP_SetClockHsrun(void)
{
    scg_sys_clk_config_t curSysClkConfig;

    CLOCK_SetHsrunModeSysClkConfig(&s_sysClkConfigSircInRun);
    /* Wait for clock source switch finished. */
    do
    {
        CLOCK_GetCurSysClkConfig(&curSysClkConfig);
    } while (curSysClkConfig.src != s_sysClkConfigSircInRun.src);
    CLOCK_InitSysPll(&s_scgSysSpllConfigHsRun);
    CLOCK_SetHsrunModeSysClkConfig(&s_sysClkConfigSpllInHsRun);
    while (!CLOCK_IsSysPllValid())
    {
    }
}

void APP_SetClockRunFromHsrun(void)
{
    scg_sys_clk_config_t curSysClkConfig;

    CLOCK_SetHsrunModeSysClkConfig(&s_sysClkConfigSircInRun);
    /* Wait for clock source switch finished. */
    do
    {
        CLOCK_GetCurSysClkConfig(&curSysClkConfig);
    } while (curSysClkConfig.src != s_sysClkConfigSircInRun.src);
    CLOCK_InitSysPll(&s_scgSysSpllConfigRun);
    while (!CLOCK_IsSysPllValid())
    {
    }
}
