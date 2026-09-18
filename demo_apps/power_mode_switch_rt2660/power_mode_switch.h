/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MODE_SWITCH_H_
#define _POWER_MODE_SWITCH_H_

#include "fsl_power.h"
#include "fsl_clock.h"

/*!
 * @addtogroup power_mode_switch_rt2660
 * @{
 *
 * Interface header only: every type/macro below is SoC-level (RT2660), not
 * board-level, so this header + power_mode_switch.c + power_mode_switch_tables.c
 * are meant to be reusable as-is by any RT2660 board variant - only app.h /
 * hardware_init.c (wiring: pins, IRQs, per-source arm/disarm) differ per board.
 *
 * The reference data tables themselves (g_powerModeTable, APP_TARGETS,
 * APP_LP_MODES, APP_LPCG_TABLE, field-setter maps, the FREQMEAS debug dump
 * tables/printers) are defined in power_mode_switch_tables.c; this header only
 * declares them (extern) plus the types/macros needed to use them.
 */

/*******************************************************************************
 * Low power mode type codes
 *
 * Used as the internal lpType argument to APP_EnterLowPower() and as indices
 * into APP_LP_MODES[] (defined in power_mode_switch_tables.c). Top-level menu
 * dispatch goes through @ref app_target_mode_t in main(); for low-power targets
 * the APP_LP_* code is the APP_TARGETS[].payload.
 ******************************************************************************/
#define APP_LP_SLEEP      1U                 /*!< Sleep - CPU-only WFI; returns via NVIC.                     */
#define APP_LP_DS1        2U                 /*!< Deep Sleep 1 - all domains retained; DCDC PWM.              */
#define APP_LP_DS2        3U                 /*!< Deep Sleep 2 - CPU retained; NPU/COMM/MEDIA off; DCDC PFM.  */
#define APP_LP_DS3        4U                 /*!< Deep Sleep 3 - all domains off; DCDC PFM.                   */
#define APP_LP_PD         5U                 /*!< Power Down - does not return; wakeup triggers PoR.          */
#define APP_LP_DPD1       6U                 /*!< Deep Power Down 1 - VBAT SRAM retained; power cycle.       */
#define APP_LP_DPD2       7U                 /*!< Deep Power Down 2 - VBAT SRAM lost; power cycle.           */
#define APP_LP_MODE_COUNT (APP_LP_DPD2 + 1U) /*!< APP_LP_MODES[] element count (index 0 unused). */

/*!
 * LP-mode bitmask helpers (bit = APP_LP_* code).  Used directly by
 * APP_RunLevel2WakeupMenu() in power_mode_switch.c to filter app.h's
 * APP_WAKEUP_SOURCES[] by lpType.
 */
#define APP_M(lp) (1U << (lp))
#define APP_M_DS  (APP_M(APP_LP_DS1) | APP_M(APP_LP_DS2) | APP_M(APP_LP_DS3))
#define APP_M_DPD (APP_M(APP_LP_DPD1) | APP_M(APP_LP_DPD2))

/*******************************************************************************
 * Run mode display names
 *
 * The demo uses the fsl_power driver's power_run_mode_t directly (queried via
 * POWER_GetCurrentRunMode()); it does not define its own run-mode enum.
 ******************************************************************************/
#define APP_RUN_MODE_NAME_COUNT 3U /*!< kPOWER_RunModeHp/Normal/Lp - see fsl_power.h power_run_mode_t. */

/*! Run-mode display names for the transition log, indexed by power_run_mode_t. */
extern const char *const APP_RUN_MODE_NAMES[APP_RUN_MODE_NAME_COUNT];

/*!
 * @brief Top-level menu target mode.
 *
 * One entry per row of the flat top-level menu.  Maps internally to either
 * a @ref power_run_mode_t (for the three active run modes) or an APP_LP_*
 * code + wakeup-source sub-menu (for Sleep / DS1..3 / PD / DPD1..2).
 */
typedef enum _app_target_mode
{
    kAPP_TargetNormalRun = 0U, /*!< Normal Drive Run (NP)   - 0.8 V, FBB, 800 MHz                 */
    kAPP_TargetHpRun     = 1U, /*!< Over Drive Run (HP)     - 0.9 V, FBB, 1 GHz                   */
    kAPP_TargetLpRun     = 2U, /*!< Low Performance Run (LP)- 0.8 V, ZBB, 600 MHz                 */
    kAPP_TargetSleep     = 3U, /*!< Sleep                   - CPU-only WFI; NVIC return           */
    kAPP_TargetDs1       = 4U, /*!< Deep Sleep 1            - all domains retained; DCDC PWM      */
    kAPP_TargetDs2       = 5U, /*!< Deep Sleep 2            - CPU retained; NPU/COMM/MEDIA off    */
    kAPP_TargetDs3       = 6U, /*!< Deep Sleep 3            - all domains off; DCDC PFM           */
    kAPP_TargetPowerDown = 7U, /*!< Power Down              - resets on wakeup                    */
    kAPP_TargetDpd1      = 8U, /*!< Deep Power Down 1       - VBAT SRAM retained; power cycle     */
    kAPP_TargetDpd2      = 9U, /*!< Deep Power Down 2       - VBAT SRAM lost; power cycle         */
    kAPP_TargetCount     = 10U
} app_target_mode_t;

/*!
 * @brief Top-level target descriptor, indexed by app_target_mode_t.
 *
 * Single source of truth for the menu label, the transition-log name, and the
 * payload: a power_run_mode_t for the three Run targets, or an APP_LP_* code for
 * the low-power targets.  main() dispatches by target index (Run vs low-power);
 * there is no per-target dispatch function pointer.
 */
typedef struct app_target
{
    const char *name;      /*!< Level-1 menu label (long).                */
    const char *shortName; /*!< Transition-log name (short).              */
    uint8_t payload;       /*!< Run: power_run_mode_t; LP: APP_LP_* code.  */
} app_target_t;

/*! Flat top-level target table, indexed by app_target_mode_t. */
extern const app_target_t APP_TARGETS[kAPP_TargetCount];
#define APP_TARGET_COUNT ((uint8_t)kAPP_TargetCount)

/*******************************************************************************
 * LPCG mode type and active-mode load-test table
 *
 * In each active run mode the demo programs all peripheral LPCGs listed in
 * APP_LPCG_TABLE[] to their configured LPCG_CFG mode value, exercising
 * maximum active-mode current load on every power supply rail.
 ******************************************************************************/

/*!
 * @brief LPCG_CFG mode values for CGC_ROOTn_SLICE_CONTROL bits[1:0].
 *
 * Per RT2660 RM Ch.20 LPCG_CFG field:
 *   0 - bus clock OFF, functional clock OFF
 *   1 - bus clock ON (OFF in Sleep), functional clock ON (OFF in Sleep)
 *   2 - bus clock ON (OFF in DeepSleep), functional clock ON (OFF in DeepSleep)
 *   3 - bus clock ON (OFF in DeepSleep), functional clock ON
 *
 * CLOCK_EnableClock() always uses mode 3 (CCM_SLICE_CONTROL_LPCG_CFG_MASK = 0x3U).
 */
typedef enum _app_lpcg_mode
{
    kAPP_LpcgModeOff            = 0U, /*!< Bus and functional clocks OFF.                              */
    kAPP_LpcgModeOffInSleep     = 1U, /*!< Clocks ON in active; gated automatically in Sleep + deeper. */
    kAPP_LpcgModeOffInDeepSleep = 2U, /*!< Clocks ON in active and Sleep; gated in DeepSleep.          */
    kAPP_LpcgModeOn             = 3U, /*!< Bus clock ON (gated in DeepSleep); functional clock ON.     */
} app_lpcg_mode_t;

/*!
 * @brief Encode a 3-bit CMC handshake-unit selection from individual enables.
 *
 * Produces the value for:
 * - CCM_SLICE_CONTROL HSK_SEL[18:16] (used in APP_LPCG_TABLE.hskSel), and
 * - POWERCON SOC_CTRL RCGCFG_HSK_SEL / CSRCCFG_HSK_SEL 3-bit sub-fields
 *   (used in power_mode_cell_t.hskSel for CSRCCFG / RCGCFG resources).
 *
 * Each CMC arg: 1 = include this CMC in the handshake, 0 = exclude.
 * RT2660 reference handshake routing: CMC0 = COMPUTE/MAIN, CMC1 = WAKE, CMC2 = COMM.
 * Default reference value: APP_HSKSEL(1,1,1) = 0x7U (all three CMCs).
 */
#define APP_HSKSEL(cmc0, cmc1, cmc2) ((uint8_t)(((cmc0) ? 1U : 0U) | ((cmc1) ? 2U : 0U) | ((cmc2) ? 4U : 0U)))

/*!
 * Short value aliases shared by power_mode_switch_tables.c (building
 * g_powerModeTable cells) and power_mode_switch_debug.c (APP_FormatCellValue(),
 * interpreting those same cell values for the debug dump) - kept here rather
 * than duplicated in both .c files.
 */
#define DCDC_PWM kPMU_DcdcModePWM           /*!< DCDC: PWM (higher power, lower noise). */
#define DCDC_PFM kPMU_DcdcModePFM           /*!< DCDC: PFM (lower power).           */
#define LDO_HP   kPMU_LdoModeHP             /*!< LDO: high-performance mode.        */
#define LDO_LP   kPMU_LdoModeLP             /*!< LDO: low-power mode.               */
#define MEM_ACT  kMEMCON_PowerModeActive    /*!< SRAM: fully active.                */
#define MEM_RET  kMEMCON_PowerModeRetention /*!< SRAM: data retained, power reduced. */
#define MEM_PD   kMEMCON_PowerModePowerDown /*!< SRAM: powered down, data lost.     */

/* PMU mode (PMUCFG.PMU_MODE bitfield) aliases for the kPOWER_Resource_PmuMode row.
 * These mirror the RT2660 PMU mode encoding used by the power driver
 * (fsl_power.c POWER_PMU_MODE_*): 0 = HP, 2 = LP, 3 = RET, 4 = Body Bias. */
#define PMU_HP  0U /*!< PMU mode: High Performance (HP/Normal run, FBB). */
#define PMU_LP  2U /*!< PMU mode: Low Power (ZBB, 0.8 V DCDC).           */
#define PMU_RET 3U /*!< PMU mode: Retention (DCDC off).                  */
#define PMU_BB  4U /*!< PMU mode: Body Bias (Deep Power Down).           */

/*! @brief One entry in the application LPCG configuration table. */
typedef struct
{
    clock_ip_name_t lpcg; /*!< LPCG identifier (clock_lpcg_t enum in fsl_clock.h). */
    app_lpcg_mode_t mode; /*!< Target LPCG_CFG value (CCM_SLICE_CONTROL bits[1:0]). */
    uint8_t hskSel;       /*!< HSK_SEL: CCM_SLICE_CONTROL bits[18:16].
                           *   Built with APP_HSKSEL(cmc0,cmc1,cmc2).
                           *   Selects which CMC instance(s) drive the clock-gate
                           *   handshake for this LPCG during low-power transitions.
                           *   Must match the routing in power_handshake_routing_config_t. */
    bool bypassHandshake; /*!< If true, sets CCM_SLICE_CONTROL_HSK_BYPASS (bit 8) to bypass
                           *   the CMC clock-gate handshake for this LPCG.
                           *   Set to true ONLY for early bring-up when the downstream CMC
                           *   handshake partner is not yet functional.  Default: false. */
} app_lpcg_config_t;

/*!
 * @brief Peripheral LPCG table for active-mode load test, terminated
 * by a sentinel with lpcg == kCLOCK_IpInvalid.  See power_mode_switch_tables.c
 * for the full per-entry rationale (hskSel routing, bypassHandshake exceptions).
 */
extern const app_lpcg_config_t APP_LPCG_TABLE[];

/*! @} */

/*******************************************************************************
 * Resource x Mode configuration table
 *
 * 2-D reference table: rows = configurable resources (Y-axis),
 *                      columns = standby modes (X-axis).
 *
 * Builder functions in power_mode_switch.c iterate this table at runtime and
 * fill the corresponding POWER_Enter*() config structs:
 *   APP_BuildSleepConfig()         -> power_sleep_config_t
 *   APP_BuildDeepSleepConfig()     -> power_deep_sleep_config_t  (DS1 / DS2 / DS3)
 *   APP_BuildPowerDownConfig()     -> power_down_config_t
 *   APP_BuildDeepPowerDownConfig() -> power_deep_power_down_config_t (DPD1 / DPD2)
 ******************************************************************************/

/*! Standby mode column indices - X-axis of g_powerModeTable.
 *
 * Each column encodes a unique (standby_mode x wakeup_source) combination so
 * that per-column clock and power configuration is fully explicit in the table
 * with no runtime overrides inside the builder functions.
 *
 * Sleep uses one column. Deep Sleep keeps one column per depth (DS1/DS2/DS3);
 * DMA-specific standby-clock differences are applied as runtime overrides in
 * APP_BuildConfig() so the table stays mode-centric instead of wake-source-centric.
 */
typedef enum _app_standby_mode_idx
{
    kAPP_StandbyModeIdx_Sleep = 0U, /*!< Sleep - wakeup source does not affect clock config. */
    kAPP_StandbyModeIdx_DS1   = 1U, /*!< Deep Sleep 1 - all domains retained. */
    kAPP_StandbyModeIdx_DS2   = 2U, /*!< Deep Sleep 2 - CPU retained, NPU/COMM/MEDIA off. */
    kAPP_StandbyModeIdx_DS3   = 3U, /*!< Deep Sleep 3 - all domains off. */
    kAPP_StandbyModeIdx_PD    = 4U, /*!< Power Down - advisory column: only the WakeSram and
                                     *   ComputeSram01 rows are meaningful here (they are the
                                     *   only two blocks written in PD).  All other rows are
                                     *   _NC_.  Cells are display-only; the driver hardcodes
                                     *   both memories to Retention in POWER_EnterPowerDown(). */
    /* Deep Power Down is NOT a table column: its retention config is a single scalar
     * (retainVbatSram) inlined directly in APP_BuildDeepPowerDownConfig(). */
    kAPP_StandbyModeIdx_COUNT = 5U
} app_standby_mode_idx_t;

/*! Configurable resource row identifiers - Y-axis of g_powerModeTable. */
typedef enum _power_standby_resource
{
    /* Power domain events - PDCON.PDSLPCFG */
    kPOWER_Resource_CpuDomain   = 0U,
    kPOWER_Resource_NpuDomain   = 1U,
    kPOWER_Resource_CommDomain  = 2U,
    kPOWER_Resource_MediaDomain = 3U,

    /* [EXPERIMENTAL] PMU analog state - POWERCON_SOC_CTRL.PMUCFG_STBY */
    kPOWER_Resource_PmuMode        = 4U,
    kPOWER_Resource_DcdcMode       = 5U,
    kPOWER_Resource_CoreLvl        = 6U,
    kPOWER_Resource_Ldo0V8Mode     = 7U,
    kPOWER_Resource_Ldo1V8Mode     = 8U,
    kPOWER_Resource_LdoVdda1V8Mode = 9U,
    kPOWER_Resource_Hqref          = 10U,
    kPOWER_Resource_Sensors        = 11U,

    /* [EXPERIMENTAL] Clock source standby gating - POWERCON_SOC_CTRL.CSRCCFG_STBY */
    kPOWER_Resource_ClkLdoa0V8  = 12U,
    kPOWER_Resource_ClkFro192M  = 13U,
    kPOWER_Resource_ClkFro12M   = 14U,
    kPOWER_Resource_ClkMainPll  = 15U,
    kPOWER_Resource_ClkCorePll  = 16U,
    kPOWER_Resource_ClkSysPll   = 17U,
    kPOWER_Resource_ClkLdoq0V8  = 18U,
    kPOWER_Resource_ClkSxosc    = 19U,
    kPOWER_Resource_ClkFro12MLp = 20U,

    /* [EXPERIMENTAL] Memory slice standby modes - MEMCON_SLICE.MEM_SLPCFG.
     * All fsl_power-managed memcon blocks are configurable here. Two blocks have
     * hardware retention paths that give them a wider valid range than the rest:
     *   - kPOWER_Resource_WakeSram (S0 B0):     WAKE_SS is always-on across
     *     Sleep/DS/PD, so Active is safe in DS and is the default. PD supports
     *     {Retention, PowerDown} (Active is semantically null in PD).
     *   - kPOWER_Resource_ComputeSram01 (S2 B0): first-class always-retained
     *     slot; Retention default. WARNING: PowerDown in DS is a foot-gun (block
     *     holds the first 64 KB of OCRAM0, commonly stack/heap) - the driver
     *     does not police the value.
     * The other 7 SRAMCTL memory blocks (S2 B1-B7) accept {Retention, PowerDown}
     * only (Active is not valid on those blocks in DS). */
    kPOWER_Resource_WakeSram      = 21U, /*!< WAKE_SS SRAM 8KB (S0 B0)  */
    kPOWER_Resource_ComputeSram01 = 22U, /*!< OCRAM0 Memory 0/1 (S2 B0) */
    kPOWER_Resource_Ocram0M23     = 23U, /*!< OCRAM0 Memory 2/3 (S2 B1) */
    kPOWER_Resource_Ocram0M4      = 24U, /*!< OCRAM0 Memory 4   (S2 B2) */
    kPOWER_Resource_Ocram0M5      = 25U, /*!< OCRAM0 Memory 5   (S2 B3) */
    kPOWER_Resource_Ocram1M0      = 26U, /*!< OCRAM1 Memory 0   (S2 B4) */
    kPOWER_Resource_Ocram1M1      = 27U, /*!< OCRAM1 Memory 1   (S2 B5) */
    kPOWER_Resource_Ocram2M0      = 28U, /*!< OCRAM2 Memory 0   (S2 B6) */
    kPOWER_Resource_Ocram2M1      = 29U, /*!< OCRAM2 Memory 1   (S2 B7) */
    kPOWER_Resource_LlcCache      = 30U,
    kPOWER_Resource_Audio8Kb      = 31U,
    kPOWER_Resource_M85Tcm        = 32U,
    kPOWER_Resource_NpuTcm        = 33U,

    /* Power Down / Deep Power Down retention are NOT table resources: those scalars
     * (retainOcram0M01 / retainVbatSram) are inlined in the PD/DPD builders. */

    /* [EXPERIMENTAL] Root clock standby enable - POWERCON_SOC_CTRL.RCGCFG_STBY */
    /* bit=1 means clock ENABLED in standby (active-high, opposite of CSRCCFG_STBY). */
    kPOWER_Resource_RcgCompute = 34U, /*!< COMPUTE_SS root clock (bit 0) */
    kPOWER_Resource_RcgMain    = 35U, /*!< MAIN_SS root clock (bit 1) */
    kPOWER_Resource_RcgWake    = 36U, /*!< WAKE_SS root clock (bit 2) */
    kPOWER_Resource_RcgComm    = 37U, /*!< COMM_SS root clock (bit 3) */
    kPOWER_Resource_RcgMedia   = 38U, /*!< MEDIA_SS root clock (bit 4) */
    kPOWER_Resource_RcgAudio   = 39U, /*!< AUDIO_SS root clock (bit 5) */
    kPOWER_Resource_RcgWake1M  = 40U, /*!< WAKE_1M root clock (bit 6) */
    kPOWER_Resource_RcgWake2M  = 41U, /*!< WAKE_2M root clock (bit 7) */

    kPOWER_Resource_COUNT = 42U
} power_standby_resource_t;

/*!
 * @brief One cell in the resource x mode table.
 *
 * valid=false (written as _NC_) means NOT_CONFIGURABLE for this mode.
 * The builder leaves the corresponding struct field at 0.
 *
 * value holds the typed config value cast to uint32_t.
 * The builder casts it back to the correct field type when writing the struct.
 */
typedef struct
{
    bool valid;     /*!< true = configurable in this mode; false = NOT_CONFIGURABLE. */
    uint32_t value; /*!< Config value cast to uint32_t; 0 when !valid. */
    uint8_t hskSel; /*!< POWERCON HSK_SEL 3-bit field for this resource's CMC routing.
                     *   Built with APP_HSKSEL(cmc0,cmc1,cmc2).  Only meaningful for
                     *   kPOWER_Resource_Clk* (CSRCCFG) and kPOWER_Resource_Rcg*
                     *   (RCGCFG) rows; ignored for all other resource types.
                     *   Used by APP_ApplyHandshakeRoutingForMode() to call POWER_SetHandshakeRouting()
                     *   with the correct per-mode CMC routing before mode entry. */
} power_mode_cell_t;

/*! Resource x Mode 2-D configuration table; see power_mode_switch_tables.c for
 *  every cell value and the _C()/_NC_/ON()/OFF() macros used to build them. */
extern const power_mode_cell_t g_powerModeTable[kPOWER_Resource_COUNT][kAPP_StandbyModeIdx_COUNT];

/*!
 * @brief Per-APP_LP_* descriptor: everything APP_EnterLowPower() and friends
 * need to know about one low-power mode, indexed by APP_LP_* code.
 *
 * Single source of truth in place of separate name/sequence/driver-mode/
 * standby-column tables - adding a mode only means adding one row here.
 */
typedef struct
{
    const char *name;                  /*!< Display name (log line + Level 2 menu header). */
    bool deinitConsole;                /*!< DbgConsole_Deinit before entry, re-init after (if it returns). */
    const char *enterNote;             /*!< "[step]" note logged before entry.                */
    const char *wakeNote;              /*!< "[step]" note logged after wake (returning modes). */
    power_low_power_mode_t driverMode; /*!< POWER_EnterLowPowerMode() dispatch target. */
    app_standby_mode_idx_t standbyIdx; /*!< g_powerModeTable column; meaningful for Sleep/DS1/DS2/DS3 only. */
} app_lp_mode_t;

/*! Per-mode descriptors, indexed by APP_LP_* code (index 0 unused). */
extern const app_lp_mode_t APP_LP_MODES[APP_LP_MODE_COUNT];

/*!
 * @brief A field setter for a Sleep/DeepSleep or PowerDown config struct: writes
 * @p value (cast to the field's real type) into the field it was generated for.
 */
typedef void (*app_field_setter_t)(void *cfg, uint32_t value);

typedef struct
{
    app_field_setter_t setter; /*!< NULL = resource has no field in this struct. */
} app_field_map_entry_t;

/*!
 * @brief Sleep/DeepSleep and PowerDown field-setter maps, indexed by
 * power_standby_resource_t.  Built in power_mode_switch_tables.c from the
 * APP_SLP_FIELD_LIST/APP_PD_FIELD_LIST X-macros; a NULL setter means the
 * resource has no field in that struct (APP_ApplyFieldMap() skips it).
 */
extern const app_field_map_entry_t g_slpFieldMap[kPOWER_Resource_COUNT];
extern const app_field_map_entry_t g_pdFieldMap[kPOWER_Resource_COUNT];

/*!
 * Maps a kPOWER_Resource_Clk* or kPOWER_Resource_Rcg* resource to the
 * POWERCON HSK_SEL register word and 3-bit field position it controls.
 *
 * regIdx:  0 = rcgcfgHskSel
 *          1 = csrccfgHskSel
 *          2 = csrccfgHskSel1
 * shift:   nibble-aligned bit offset of the 3-bit HSK_SEL field (0,4,8,...28).
 *
 * Non-handshake-routing resources have {0,0} (zero-initialized); they are never
 * accessed because their power_mode_cell_t.hskSel is always 0.
 */
typedef struct
{
    uint8_t regIdx; /*!< POWERCON HSK_SEL register index: 0=rcgcfg, 1=csrccfg, 2=csrccfg1 */
    uint8_t shift;  /*!< Nibble-aligned bit offset of the 3-bit HSK_SEL field (0,4,...28). */
} app_hsk_field_t;

extern const app_hsk_field_t g_hskFieldMap[kPOWER_Resource_COUNT];

/*!
 * @brief SoC handshake routing - HSK_SEL routing and PDCON per-domain handshake mask.
 *
 * Defined in power_mode_switch_tables.c; applied by BOARD_InitHardware() (board
 * hardware_init.c, via the extern declaration in that board's app.h) as the
 * final step of bring-up, after BOARD_CommonSetting() applies POWER_SetPolicy()
 * with POR-default routing.  To customise handshake routing for a non-reference
 * board design, edit the matching field of g_handshakeRoutingCfg in
 * power_mode_switch_tables.c; keep the hskSel field of every affected
 * APP_LPCG_TABLE entry in sync so CCM SLICE_CONTROL.HSK_SEL matches.
 */
extern power_handshake_routing_config_t g_handshakeRoutingCfg;

/*!
 * @brief Debug-only dumps, compiled in only when APP_PRINT_DEBUG_INFO is nonzero.
 *
 * Definitions (power_mode_switch_debug.c) and call sites (power_mode_switch.c)
 * are separate translation units, so the macro must resolve identically in
 * both: default it here (single canonical place) rather than duplicating the
 * ifndef guard in each .c file. A board that wants the dumps enabled defines
 * APP_PRINT_DEBUG_INFO in its app.h, included before this header in both .c
 * files, so the override below is skipped in favor of the board's value.
 *
 * Both print from tables owned by power_mode_switch_tables.c (measured-clock
 * name tables, g_powerModeTable) - the tables themselves are not exposed here
 * since nothing outside that file needs them.
 */
#ifndef APP_PRINT_DEBUG_INFO
#define APP_PRINT_DEBUG_INFO 0
#endif

void APP_PrintMeasuredClocks(void);
void APP_PrintModeTable(void);

#endif /* _POWER_MODE_SWITCH_H_ */
