/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * RT2660 Power Mode Switch Demo
 * ==============================
 * Two-level interactive menu demonstrating all RT2660 power modes.
 *
 * Level 1 - low power mode selection:
 *   Run Mode Switch, Sleep, Deep Sleep 1-3, Power Down, Deep Power Down 1-2.
 *
 * Level 2 - wakeup source selection per mode:
 *   Deep Power Down modes show a VBAT-domain Level 2 menu (VBAT LPTMR or SW5).
 *   Run Mode Switch shows a list of target run modes instead.
 *
 * Mode-specific wakeup source tables are defined in app.h (board wiring); power
 * config presets and menu strings are defined in power_mode_switch.h.
 *
 * Run mode switches go through the driver's unified POWER_EnterRunMode(), which
 * dispatches to POWER_EnterHpRun / POWER_EnterNormalRun / POWER_EnterLpRun; the
 * current mode is queried from the power driver via POWER_GetCurrentRunMode()
 * (power_run_mode_t).
 *
 * Boot origin detection after Power Down or Deep Power Down is the
 * application's responsibility (e.g., inspect POWERCON WARM_GPR[0] or
 * VBATCON status registers directly).  The fsl_power driver does not provide
 * a POWER_GetWakeupReason() API.
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_power.h"
#include "fsl_clock.h"
#include "fsl_lpuart.h"
#include "app.h"
#include "board.h"
#include "power_mode_switch.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Wakeup source control (per-source enable/disable + framework) */
static void APP_EnableWakeup(uint8_t src);
static void APP_DisableWakeup(uint8_t src);

/* Run mode helpers */
static void APP_SwitchRunMode(power_run_mode_t target);
static void APP_WaitTxDone(void);

/* Flat top-level menu + wakeup-source sub-menu.
 * APP_PrintMeasuredClocks()/APP_PrintModeTable() are declared in power_mode_switch.h
 * and defined in power_mode_switch_debug.c, which compiles them out entirely
 * when APP_PRINT_DEBUG_INFO is 0; the call sites here (APP_PrintTopMenu()) are
 * gated by the same macro so both sides agree. */
static void APP_PrintTopMenu(void);
static uint8_t APP_ReadMenuSelection(void);
static uint8_t APP_RunLevel2WakeupMenu(uint8_t lpType);
static void APP_EnterLowPower(const app_target_t *t);

/* Transition-chain logging helpers (shared by APP_SwitchRunMode and APP_EnterLowPower) */
static void APP_BeginTransition(const char *fromName, const char *toName);
static void APP_LogStep(const char *fromName, const char *toName, const char *note);
static void APP_LogWakeEvent(const char *wakeSourceName);

/* LPCG load test helpers */
static void APP_ApplyLpcgTable(const app_lpcg_config_t *table);

/* Table-driven config builders - parse g_powerModeTable and fill POWER_Enter*() structs */
static void APP_BuildSleepConfig(power_sleep_config_t *cfg);
static void APP_BuildDeepSleepConfig(app_standby_mode_idx_t modeIdx, power_deep_sleep_config_t *cfg);
static void APP_BuildPowerDownConfig(power_down_config_t *cfg);
static void APP_BuildDeepPowerDownConfig(uint8_t lpType, power_deep_power_down_config_t *cfg);

/* Build every low-power mode's config once at startup (see the s_*Cfg globals below),
 * instead of re-walking g_powerModeTable on every menu selection. */
static void APP_BuildAllConfigs(void);

/* Handshake-routing builder - extracts hskSel from g_powerModeTable and calls POWER_SetHandshakeRouting */
static void APP_ApplyHandshakeRoutingForMode(app_standby_mode_idx_t modeIdx);

/* Low-power entry: prepare config -> arm -> enter -> handle wakeup (see APP_EnterLowPower) */
static const void *APP_PrepareLowPowerConfig(uint8_t lpType);
static void APP_EnterLowPowerWithCfg(uint8_t lpType, const void *cfg);
static void APP_HandleLowPowerWakeup(uint8_t lpType, uint8_t wakeupSrc);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*
 * Every low-power mode's fsl_power config, built once by APP_BuildAllConfigs() at
 * startup instead of on every menu selection.
 */
static power_sleep_config_t s_sleepCfg;
static power_deep_sleep_config_t s_dsCfg[3];       /* [0]=DS1 [1]=DS2 [2]=DS3 */
static power_down_config_t s_pdCfg;
static power_deep_power_down_config_t s_dpdCfg[2]; /* [0]=DPD1 [1]=DPD2 */

/*******************************************************************************
 * Main
 ******************************************************************************/
int main(void)
{
    uint8_t idx;

    BOARD_InitHardware();

    /*
     * Boot origin detection after Power Down or Deep Power Down is the
     * application's responsibility.  The fsl_power driver does not provide a
     * POWER_GetWakeupReason() API.  Applications may inspect POWERCON
     * WARM_GPR[0] or VBATCON status registers to determine boot origin.
     */
    PRINTF("\r\nPower Mode Switch Example Started...\r\n");

    /* If this boot is a Power Down / Deep Power Down wake (a PoR), acknowledge and clear
     * the latched VBAT wakeup source so a stale flag does not immediately re-trigger the
     * next low-power entry. */
    bool wokeFromDpd = BOARD_HandleWakeupAfterReset();

    /* VBAT_SRAM retention self-check: only meaningful after a DPD1 wake - only DPD1
     * arms the marker before entry (see APP_EnterLowPowerWithCfg). DPD2 shuts
     * VBAT__SRAM down before entry and nothing re-powers it before this point, so
     * touching it here for a DPD2 wake reads a powered-down SRAM array. RESETCON
     * cannot tell DPD1 apart from DPD2, so consult the VBATCON GPR marker written
     * just before entry instead. Skipped entirely after Sleep/DS/PD wake or a cold
     * boot (wokeFromDpd covers both). */
    if (wokeFromDpd && (BOARD_GetAndClearDpdVariantMarker() == BOARD_DPD_VARIANT_DPD1))
    {
        BOARD_VbatSramReport();
    }

    /* Enable all peripheral LPCGs per the load-test table.
     * In production, replace kAPP_LpcgModeOn with the desired gating policy
     * per peripheral.  Keep always-on entries (CPU clock, power sequencer)
     * at kAPP_LpcgModeOn regardless of the active mode under test. */
    APP_ApplyLpcgTable(APP_LPCG_TABLE);

    /* Build every low-power mode's fsl_power config once, up front: walking
     * g_powerModeTable is more work than we want to repeat every time the same
     * mode is picked from the menu loop below. */
    APP_BuildAllConfigs();

    while (true)
    {
        APP_PrintTopMenu();

        idx = APP_ReadMenuSelection();
        if (idx == 0xFFU)
        {
            continue;
        }

        const app_target_t *t = &APP_TARGETS[idx];
        if (idx <= (uint8_t)kAPP_TargetLpRun)
        {
            /* Run target: switch run mode immediately (no wakeup sub-menu). */
            APP_BeginTransition(APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()], t->shortName);
            APP_SwitchRunMode((power_run_mode_t)t->payload);
            PRINTF("\r\nCurrent mode: %s\r\n", APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()]);
        }
        else
        {
            /* Low-power target: wakeup menu + entry sequence. */
            APP_EnterLowPower(t);
        }

        PRINTF("Next Loop...\r\n");
    }
}

/*******************************************************************************
 * Wakeup source control
 *
 * Each board-specific per-source enable()/disable() (defined in hardware_init.c)
 * owns its FULL arm/disarm: peripheral-specific bits plus EnableIRQ/DisableIRQ +
 * POWER_Enable/DisableWakeupSource. Not split with a generic dispatcher applying
 * the NVIC/POWERCON half from a single irq/powerSrc table field, because some
 * sources need more than one such registration (e.g. across different CMC
 * domains) - a per-source function has no fixed cardinality limit.
 ******************************************************************************/

/*!
 * @brief Enable a wakeup source, bracketed by POWER_ClearAllWakeupSources().
 *
 * Used at every low-power entry (Sleep / Deep Sleep / Power Down / Deep Power Down).
 *
 * Bring-up note: hardware_init.c's enable() functions program the POWERCON mask
 * for every mode, including Sleep (NVIC still handles the actual wakeup - the
 * mask is harmless) and Deep Power Down (armed alongside the VBATCON latch, even
 * though DPD wakeup is actually driven by the VBATCON latch alone).
 */
static void APP_EnableWakeup(uint8_t src)
{
    const app_wakeup_source_t *ws = &APP_WAKEUP_SOURCES[src];

    POWER_ClearAllWakeupSources();

    if (ws->enable != NULL)
    {
        ws->enable();
    }
    /* Record which source was enabled in a VBATCON GPR (survives PD/DPD PoR).
     * BOARD_HandleWakeupAfterReset() uses this on the next boot to report the
     * actual cause instead of whichever of LPTMR/RTC happens to have a
     * latched status flag - which may be stale from an earlier test. */
    BOARD_SetEnabledWakeupMarker(src);
}

/*!
 * @brief Tear down a wakeup source, bracketed by POWER_ClearAllWakeupSources().
 */
static void APP_DisableWakeup(uint8_t src)
{
    const app_wakeup_source_t *ws = &APP_WAKEUP_SOURCES[src];

    if (ws->disable != NULL)
    {
        ws->disable();
    }
    POWER_ClearAllWakeupSources();
}

/*******************************************************************************
 * Run mode helpers
 *
 * The demo queries the current run mode directly via POWER_GetCurrentRunMode()
 * (power_run_mode_t); there is no example-level run-mode enum or cached copy.
 ******************************************************************************/
/*!
 * @brief Per-user-pick step counter for the transition-chain printer.
 *
 * Reset at the start of every top-level mode dispatch.  Each call to
 * APP_LogStep() increments it and prefixes the printed line with the count.
 */
static uint8_t s_stepNum;

static void APP_BeginTransition(const char *fromName, const char *toName)
{
    s_stepNum = 0U;
    PRINTF("\r\nTransition: %s -> %s\r\n", fromName, toName);
}

static void APP_LogStep(const char *fromName, const char *toName, const char *note)
{
    s_stepNum++;
    if ((note != NULL) && (note[0] != '\0'))
    {
        PRINTF("  [step %u] %s -> %s  (%s)\r\n", (uint32_t)s_stepNum, fromName, toName, note);
    }
    else
    {
        PRINTF("  [step %u] %s -> %s\r\n", (uint32_t)s_stepNum, fromName, toName);
    }
}

static void APP_LogWakeEvent(const char *wakeSourceName)
{
    if ((wakeSourceName != NULL) && (wakeSourceName[0] != '\0'))
    {
        PRINTF("  [wake event: %s]\r\n", wakeSourceName);
    }
    else
    {
        PRINTF("  [wake event]\r\n");
    }
}

/* Maps power_run_mode_t to the board clock-ramp callback POWER_EnterRunMode()
 * should invoke for that target. */
static const power_clock_cb_t s_runModeClockCb[] = {
    [kPOWER_RunModeHp]     = BOARD_BootClockHPRUN,
    [kPOWER_RunModeNormal] = BOARD_BootClockNPRUN,
    [kPOWER_RunModeLp]     = BOARD_BootClockLPRUN,
};

static void APP_SwitchRunMode(power_run_mode_t target)
{
    const char *fromName = APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()];
    const char *toName   = APP_RUN_MODE_NAMES[target];

    if (POWER_GetCurrentRunMode() == target)
    {
        APP_LogStep(fromName, toName, "already in target run mode");
        return;
    }
    POWER_EnterRunMode(target, s_runModeClockCb[target]);
    APP_LogStep(fromName, toName, "run mode switched");
}

static void APP_WaitTxDone(void)
{
    while ((LPUART_GetStatusFlags((LPUART_Type *)APP_UART_BASEADDR) & kLPUART_TransmissionCompleteFlag) == 0U)
    {
    }
}

/*******************************************************************************
 * LPCG load test helpers ******************************************************************************/

/*!
 * @brief Apply a sentinel-terminated LPCG configuration table.
 *
 * Iterates APP_LPCG_TABLE[] (or any conforming table) and calls
 * CLOCK_SetClockGateMode() for each entry until the sentinel
 * { kCLOCK_IpInvalid, kAPP_LpcgModeOff } is reached.
 *
 * Call once after POWER_SetPolicy() and the initial run-mode selection so that
 * all peripheral clocks are gated according to the table before the first
 * menu iteration.  For active-mode load characterisation, the default table
 * sets every LPCG to kAPP_LpcgModeOn (mode 3).
 *
 * @param table  Pointer to the first element of the LPCG config table.
 */
static void APP_ApplyLpcgTable(const app_lpcg_config_t *table)
{
    assert(table != NULL);
    while (table->lpcg != kCLOCK_IpInvalid)
    {
        CLOCK_SetClockGateMode(table->lpcg, (clock_gate_value_t)table->mode, table->hskSel, table->bypassHandshake);
        table++;
    }
}

/*!
 * @brief Apply one g_powerModeTable column to a config struct via a field-setter map.
 *
 * Shared by Sleep/DeepSleep (g_slpFieldMap) and Power Down (g_pdFieldMap): every
 * valid cell in the column is handed to its resource's setter, which writes the
 * cell's value into the matching struct field. Rows with no setter for this
 * struct (PD/DPD-only resources, or memcon rows when applying to power_down_config_t)
 * are skipped.
 */
static void APP_ApplyFieldMap(app_standby_mode_idx_t modeIdx, const app_field_map_entry_t *fieldMap, uint8_t *base)
{
    uint32_t r;
    for (r = 0U; r < (uint32_t)kPOWER_Resource_COUNT; r++)
    {
        const power_mode_cell_t *cell = &g_powerModeTable[r][(uint32_t)modeIdx];
        if (!cell->valid || (fieldMap[r].setter == NULL))
        {
            continue;
        }
        fieldMap[r].setter(base, cell->value);
    }
}

/*!
 * @brief Apply g_powerModeTable cells to a Sleep or Deep Sleep config struct.
 *
 * power_sleep_config_t and power_deep_sleep_config_t share identical leading field
 * layouts, so g_slpFieldMap's power_deep_sleep_config_t setters work for both.
 *
 * @param modeIdx  Column index in g_powerModeTable.
 * @param base     Pointer to the first byte of a zero-initialised config struct.
 */
static void APP_ApplySleepFields(app_standby_mode_idx_t modeIdx, uint8_t *base)
{
    assert(base != NULL);
    APP_ApplyFieldMap(modeIdx, g_slpFieldMap, base);
}

/*!
 * @brief Build a power_sleep_config_t from the Sleep column of g_powerModeTable.
 */
static void APP_BuildSleepConfig(power_sleep_config_t *cfg)
{
    assert(cfg != NULL);
    (void)memset(cfg, 0, sizeof(*cfg));
    APP_ApplySleepFields(kAPP_StandbyModeIdx_Sleep, (uint8_t *)cfg);
}

/*!
 * @brief Build a power_deep_sleep_config_t from the specified DS column of g_powerModeTable.
 *
 * All column values are read directly from the table with no runtime overrides.
 * The clock sources DMA wakeup needs are configured by BOARD_EnableDmaWake()
 * itself (hardware_init.c), not here.
 *
 * @param modeIdx  DS column to use (one of kAPP_StandbyModeIdx_DS*).
 * @param cfg      Output config struct.  Must not be NULL.
 */
static void APP_BuildDeepSleepConfig(app_standby_mode_idx_t modeIdx, power_deep_sleep_config_t *cfg)
{
    assert(cfg != NULL);
    (void)memset(cfg, 0, sizeof(*cfg));
    APP_ApplySleepFields(modeIdx, (uint8_t *)cfg);
}

/*!
 * @brief Build a power_handshake_routing_config_t from the hskSel fields of g_powerModeTable.
 *
 * @param modeIdx  Column index in g_powerModeTable (Sleep, DS1_Irq, DS1_Dma, ..., PD, DPD).
 * @param routing  Output handshake routing config struct.  Must not be NULL.
 */
static void APP_BuildHandshakeRoutingConfig(app_standby_mode_idx_t modeIdx, power_handshake_routing_config_t *routing)
{
    uint32_t r;
    uint32_t *regs[3];
    assert(routing != NULL);
    POWER_GetDefaultHandshakeRoutingConfig(routing);

    regs[0] = &routing->rcgcfgHskSel;
    regs[1] = &routing->csrccfgHskSel;
    regs[2] = &routing->csrccfgHskSel1;

    for (r = 0U; r < (uint32_t)kPOWER_Resource_COUNT; r++)
    {
        const power_mode_cell_t *cell = &g_powerModeTable[r][(uint32_t)modeIdx];
        if (!cell->valid || (cell->hskSel == 0U))
        {
            continue; /* not a handshake-routing resource or not configured for this mode */
        }
        const app_hsk_field_t *f = &g_hskFieldMap[r];
        *regs[f->regIdx]         = (*regs[f->regIdx] & ~(0x7UL << f->shift)) | ((uint32_t)cell->hskSel << f->shift);
    }
}

/*!
 * @brief Apply POWERCON handshake routing for the given mode before calling POWER_Enter*().
 *
 * Builds a power_handshake_routing_config_t from the hskSel fields of g_powerModeTable
 * for @p modeIdx and writes it to hardware via POWER_SetHandshakeRouting().  Must be
 * called after APP_Build*Config() and before POWER_Enter*().
 *
 * @param modeIdx  Standby mode column (kAPP_StandbyModeIdx_Sleep, etc.).
 */
static void APP_ApplyHandshakeRoutingForMode(app_standby_mode_idx_t modeIdx)
{
    power_handshake_routing_config_t routing;
    APP_BuildHandshakeRoutingConfig(modeIdx, &routing);
    POWER_SetHandshakeRouting(&routing);
}

/*!
 * @brief Build a power_down_config_t from the PD column of g_powerModeTable.
 *
 * Consumes the two configurable rows (WakeSram, ComputeSram01) via
 * APP_ApplyFieldMap(). See design.md Memory Regions - Power Down for the
 * valid-values discussion.
 */
static void APP_BuildPowerDownConfig(power_down_config_t *cfg)
{
    assert(cfg != NULL);
    (void)memset(cfg, 0, sizeof(*cfg));
    APP_ApplyFieldMap(kAPP_StandbyModeIdx_PD, g_pdFieldMap, (uint8_t *)cfg);
}

/*!
 * @brief Build a power_deep_power_down_config_t from the DPD column.
 *
 * DPD1 (retainVbatSram=true):  table value used as-is.
 * DPD2 (retainVbatSram=false): override applied after table read.
 *
 * @param lpType  APP_LP_DPD1 or APP_LP_DPD2.
 */
static void APP_BuildDeepPowerDownConfig(uint8_t lpType, power_deep_power_down_config_t *cfg)
{
    assert(cfg != NULL);
    (void)memset(cfg, 0, sizeof(*cfg));
    /* DPD1 retains the VBAT SRAM (VDD_PMU kept); DPD2 does not (PMIC_CTRL asserted,
     * VDD_PMU removed). Previously the DPD column of g_powerModeTable + a DPD2 override. */
    cfg->retainVbatSram = (lpType != APP_LP_DPD2);
}

/*******************************************************************************
 * Sequence execution
 ******************************************************************************/

static void APP_BuildAllConfigs(void)
{
    APP_BuildSleepConfig(&s_sleepCfg);
    APP_BuildDeepSleepConfig(kAPP_StandbyModeIdx_DS1, &s_dsCfg[0]);
    APP_BuildDeepSleepConfig(kAPP_StandbyModeIdx_DS2, &s_dsCfg[1]);
    APP_BuildDeepSleepConfig(kAPP_StandbyModeIdx_DS3, &s_dsCfg[2]);
    APP_BuildPowerDownConfig(&s_pdCfg);
    APP_BuildDeepPowerDownConfig(APP_LP_DPD1, &s_dpdCfg[0]);
    APP_BuildDeepPowerDownConfig(APP_LP_DPD2, &s_dpdCfg[1]);
}

/*!
 * @brief Apply handshake routing and resolve the fsl_power config for lpType,
 * ready to hand to APP_EnterLowPowerWithCfg().
 *
 * Sleep / Deep Sleep have a g_powerModeTable column of their own, so their
 * handshake routing is rebuilt per mode. Power Down / Deep Power Down have no
 * such column - they keep whatever routing BOARD_InitHardware() already set
 * up (g_handshakeRoutingCfg), so nothing to apply here. The resolved config is
 * one of the s_*Cfg globals built by APP_BuildAllConfigs().
 *
 * @return Pointer to the resolved config, to pass straight to
 *         POWER_EnterLowPowerMode() as its lpModeConfig argument.
 */
static const void *APP_PrepareLowPowerConfig(uint8_t lpType)
{
    if ((lpType == APP_LP_SLEEP) || (lpType == APP_LP_DS1) || (lpType == APP_LP_DS2) || (lpType == APP_LP_DS3))
    {
        APP_ApplyHandshakeRoutingForMode(APP_LP_MODES[lpType].standbyIdx);
    }

    switch (lpType)
    {
        case APP_LP_SLEEP:
            return &s_sleepCfg;
        case APP_LP_DS1:
        case APP_LP_DS2:
        case APP_LP_DS3:
            return &s_dsCfg[lpType - APP_LP_DS1];
        case APP_LP_PD:
            return &s_pdCfg;
        case APP_LP_DPD1:
        case APP_LP_DPD2:
            return &s_dpdCfg[lpType - APP_LP_DPD1];
        default:
            return NULL;
    }
}

/*
 * Enter the low power mode for lpType via the driver's unified
 * POWER_EnterLowPowerMode() (the matching POWER_Enter*() only, on the pre-built
 * config).  Power Down and Deep Power Down do not return.
 */
static void APP_EnterLowPowerWithCfg(uint8_t lpType, const void *cfg)
{
    if (lpType == APP_LP_DPD1)
    {
        /* VBAT_SRAM retention test is DPD1-only: DPD2 removes VDD_PMU and the SRAM
         * is powered down by POWER_EnterDeepPowerDown() (retainVbatSram == false), so
         * there is nothing to arm or later check for DPD2. */
        BOARD_InitVbatSram();
        BOARD_EnableVbatSram();
        BOARD_SetDpdVariantMarker(BOARD_DPD_VARIANT_DPD1);
    }
    else if (lpType == APP_LP_DPD2)
    {
        BOARD_SetDpdVariantMarker(BOARD_DPD_VARIANT_DPD2);
    }
    (void)POWER_EnterLowPowerMode(APP_LP_MODES[lpType].driverMode, cfg); /* PD / DPD do not return */
}

/*!
 * @brief Post-wakeup teardown for a returning low-power mode (Sleep / Deep Sleep).
 *
 * Re-inits the console (if it was deinited before entry), disarms the wakeup
 * source, and logs the wake event and current mode.  lpType/wakeupSrc are the
 * same values APP_EnterLowPower() used to enter, so it re-derives the mode
 * descriptor here instead of threading it through as an extra parameter.
 */
static void APP_HandleLowPowerWakeup(uint8_t lpType, uint8_t wakeupSrc)
{
    const app_lp_mode_t *mode = &APP_LP_MODES[lpType];

    if (mode->deinitConsole)
    {
        BOARD_InitDebugConsole();
    }
    APP_DisableWakeup(wakeupSrc);
    APP_LogWakeEvent(APP_WAKEUP_SOURCES[wakeupSrc].name);
    APP_LogStep(mode->name, APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()], mode->wakeNote);
    PRINTF("\r\nCurrent mode: %s\r\n", APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()]);
}

/*!
 * @brief Low-power target handler (Sleep / DS / PD / DPD).
 *
 * The whole low-power path in one linear function: pick a wakeup source, then
 * the common entry flow - APP_PrepareLowPowerConfig() (handshake routing +
 * config resolve) -> log -> arm wakeup (last) -> APP_EnterLowPowerWithCfg() ->
 * on wake, APP_HandleLowPowerWakeup().  Per-mode flags/notes come from
 * APP_LP_MODES[lpType].  Power Down / Deep Power Down do not return, so the
 * post-entry tail is never reached.
 */
static void APP_EnterLowPower(const app_target_t *t)
{
    uint8_t lpType            = t->payload;
    const app_lp_mode_t *mode = &APP_LP_MODES[lpType];
    uint8_t wakeupSrc;
    const void *cfg;

    /* Level 2: pick a wakeup source. */
    PRINTF("\r\n>>> %s\r\n", t->name);
    wakeupSrc = APP_RunLevel2WakeupMenu(lpType);
    if (wakeupSrc == APP_WAKEUP_NONE)
    {
        return; /* user backed out */
    }
    APP_BeginTransition(APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()], t->shortName);

    cfg = APP_PrepareLowPowerConfig(lpType);

    APP_LogStep(APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()], mode->name, mode->enterNote);
    APP_WaitTxDone();

    if (mode->deinitConsole)
    {
        DbgConsole_Deinit();
    }
    /* Arm the wakeup source last, immediately before entry. */
    APP_EnableWakeup(wakeupSrc);
    APP_EnterLowPowerWithCfg(lpType, cfg); /* Power Down / Deep Power Down do NOT return */

    /* --- Returning modes (Sleep / Deep Sleep) resume here. --- */
    APP_HandleLowPowerWakeup(lpType, wakeupSrc);
}

/*******************************************************************************
 * Flat top-level menu
 ******************************************************************************/

/*!
 * @brief Print the flat top-level menu - one entry per supported mode.
 */
static void APP_PrintTopMenu(void)
{
    uint8_t i;
    PRINTF("\r\n");
    PRINTF("==============================================\r\n");
    PRINTF("   RT2660 Power Mode Switch Demo\r\n");
    PRINTF("   Current mode: %s\r\n", APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()]);
#if APP_PRINT_DEBUG_INFO
    APP_PrintMeasuredClocks();
    APP_PrintModeTable();
#endif
    PRINTF("==============================================\r\n");
    for (i = 0U; i < APP_TARGET_COUNT; i++)
    {
        if (i < 9U)
        {
            PRINTF("   %c. %s\r\n", (char)('1' + i), APP_TARGETS[i].name);
        }
        else
        {
            /* Entries 10+ shown as A, B, ... */
            PRINTF("   %c. %s\r\n", (char)('A' + (i - 9U)), APP_TARGETS[i].name);
        }
    }
    PRINTF("==============================================\r\n");
    PRINTF("  Select (1-9, A): ");
}

/*!
 * @brief Read one top-level menu keypress and map it to a mode-table index.
 *
 * Accepts '1'..'9' for entries 0..8 and 'A'/'a' for entry 9 (case-insensitive),
 * echoes the accepted key, and returns the entry index.  Returns 0xFF on any
 * invalid or out-of-range key (after printing "Invalid selection").
 */
static uint8_t APP_ReadMenuSelection(void)
{
    uint32_t ch = (uint32_t)GETCHAR();
    uint8_t idx;

    /* Normalize lowercase to uppercase. */
    if ((ch >= (uint32_t)'a') && (ch <= (uint32_t)'z'))
    {
        ch -= (uint32_t)('a' - 'A');
    }

    /* Flat selection: '1'..'9' for entries 0..8, 'A' for entry 9. */
    if ((ch >= (uint32_t)'1') && (ch <= (uint32_t)'9'))
    {
        idx = (uint8_t)(ch - (uint32_t)'1');
    }
    else if (ch == (uint32_t)'A')
    {
        idx = 9U;
    }
    else
    {
        PRINTF("\r\n  Invalid selection.\r\n");
        return 0xFFU;
    }

    if (idx >= APP_TARGET_COUNT)
    {
        PRINTF("\r\n  Invalid selection.\r\n");
        return 0xFFU;
    }

    PRINTF("%c\r\n", (char)ch);
    return idx;
}

/*!
 * @brief Run Level 2 wakeup-source sub-menu for the given low power mode.
 *
 * Displays the wakeup source list for @p lpType, reads the user's selection,
 * and returns the chosen APP_WAKEUP_* code.
 *
 * @return Selected APP_WAKEUP_* code, or APP_WAKEUP_NONE if the user cancels.
 */
static uint8_t APP_RunLevel2WakeupMenu(uint8_t lpType)
{
    uint8_t codes[APP_WAKEUP_SOURCE_COUNT]; /* menu index -> APP_WAKEUP_* code */
    uint8_t count = 0U;
    uint8_t i;
    uint32_t ch;

    /* Build the list from every source whose modes bitmask includes this mode. */
    for (i = 0U; i < APP_WAKEUP_SOURCE_COUNT; i++)
    {
        if ((APP_WAKEUP_SOURCES[i].modes & APP_M(lpType)) != 0U)
        {
            codes[count++] = i; /* table is indexed by code, so i == APP_WAKEUP_* code */
        }
    }
    if (count == 0U)
    {
        return APP_WAKEUP_NONE; /* no selectable source for this mode */
    }

    PRINTF("\r\n  Wakeup source:\r\n");
    for (i = 0U; i < count; i++)
    {
        PRINTF("   %c. %s\r\n", (char)('1' + i), APP_WAKEUP_SOURCES[codes[i]].name);
    }
    PRINTF("   0. Back\r\n");
    PRINTF("  Select: ");

    ch = (uint32_t)GETCHAR();
    PRINTF("%c\r\n", (char)ch);

    if (ch == (uint32_t)'0')
    {
        return APP_WAKEUP_NONE; /* back to Level 1 */
    }

    if ((ch >= (uint32_t)'1') && (ch < (uint32_t)'1' + (uint32_t)count))
    {
        return codes[(uint8_t)(ch - (uint32_t)'1')];
    }

    PRINTF("  Invalid selection.\r\n");
    return APP_WAKEUP_NONE;
}
