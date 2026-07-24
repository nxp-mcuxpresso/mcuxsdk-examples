/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MODE_SWITCH_H_
#define _POWER_MODE_SWITCH_H_

#include "fsl_power.h"
#include "fsl_clock.h"
#include "app.h"

/*!
 * @addtogroup power_mode_switch_rt2660
 * @{
 */

/*******************************************************************************
 * Low power mode type codes
 *
 * Used as the internal lpType argument to APP_EnterLowPower() and as indices
 * into s_lpRunCfg[].  Top-level menu dispatch goes through @ref app_target_mode_t
 * in main(); for low-power targets the APP_LP_* code is the APP_TARGETS[].payload.
 ******************************************************************************/
#define APP_LP_SLEEP        1U  /*!< Sleep - CPU-only WFI; returns via NVIC.                     */
#define APP_LP_DS1          2U  /*!< Deep Sleep 1 - all domains retained; DCDC PWM.              */
#define APP_LP_DS2          3U  /*!< Deep Sleep 2 - CPU retained; NPU/COMM/MEDIA off; DCDC PFM.  */
#define APP_LP_DS3          4U  /*!< Deep Sleep 3 - all domains off; DCDC PFM.                   */
#define APP_LP_PD           5U  /*!< Power Down - does not return; wakeup triggers PoR.          */
#define APP_LP_DPD1         6U  /*!< Deep Power Down 1 - VBAT SRAM retained; power cycle.       */
#define APP_LP_DPD2         7U  /*!< Deep Power Down 2 - VBAT SRAM lost; power cycle.           */

/*!
 * LP-mode bitmask helpers for app_wakeup_source_t.modes (bit = APP_LP_* code).
 * A wakeup source declares the set of low-power modes it supports; the Level 2
 * menu shows only sources whose mask includes the selected mode.
 */
#define APP_M(lp)   (1U << (lp))
#define APP_M_DS    (APP_M(APP_LP_DS1) | APP_M(APP_LP_DS2) | APP_M(APP_LP_DS3))
#define APP_M_DPD   (APP_M(APP_LP_DPD1) | APP_M(APP_LP_DPD2))

/*!
 * LP-mode display names for the "[step ...]" log line, indexed directly by
 * APP_LP_* code.  Index 0 is unused (APP_LP_* values start at 1) and stays NULL.
 */
static const char *const APP_LP_MODE_NAMES[] = {
    [APP_LP_SLEEP] = "Sleep",
    [APP_LP_DS1]   = "Deep Sleep 1",
    [APP_LP_DS2]   = "Deep Sleep 2",
    [APP_LP_DS3]   = "Deep Sleep 3",
    [APP_LP_PD]    = "Power Down",
    [APP_LP_DPD1]  = "Deep Power Down 1",
    [APP_LP_DPD2]  = "Deep Power Down 2",
};

/*!
 * @brief Config union for the low-power entry sequence.
 *
 * One member per POWER_Enter*() config type; only the member for the selected
 * mode is built and used.  Lets build() (which produces the config) and enter()
 * (which consumes it) stay separate, with the arming of the wakeup source in
 * between (build before arm; arm immediately before enter).
 */
typedef union
{
    power_sleep_config_t           sleep;
    power_deep_sleep_config_t      deepSleep;
    power_down_config_t            powerDown;
    power_deep_power_down_config_t deepPowerDown;
} app_lp_cfg_t;

/*!
 * @brief Low-power entry-sequence descriptor, indexed by APP_LP_* code.
 *
 * APP_EnterLowPower() runs one common flow for every mode; this descriptor
 * supplies only the flags and log notes that differ.  The config build and the
 * matching POWER_Enter*() are handled by APP_BuildConfig() / APP_EnterConfig()
 * (switch on lpType) in power_mode_switch.c - the config struct type varies per
 * mode, so that per-type code lives in those two helpers rather than the table.
 */
typedef struct
{
    bool        lpRunBracket;  /*!< Wrap entry with APP_Enter/ExitLowPowerRun (Deep Sleep). */
    bool        deinitConsole; /*!< DbgConsole_Deinit before entry, re-init after (if it returns). */
    const char *enterNote;     /*!< "[step]" note logged before entry.                */
    const char *wakeNote;      /*!< "[step]" note logged after wake (returning modes). */
} app_lp_seq_t;

/*! Entry-sequence descriptors, indexed by APP_LP_* code (index 0 unused). */
static const app_lp_seq_t APP_LP_SEQ[] = {
    [APP_LP_SLEEP] = { false, false, "WFI entered", "wake; run mode preserved" },
    [APP_LP_DS1]   = { true,  true,  "WFI entered", "PMU restored; LP clock tree back" },
    [APP_LP_DS2]   = { true,  true,  "WFI entered", "PMU restored; LP clock tree back" },
    [APP_LP_DS3]   = { true,  true,  "WFI entered", "PMU restored; LP clock tree back" },
    [APP_LP_PD]    = { false, true,  "WFI entered; system resets on wakeup", NULL },
    [APP_LP_DPD1]  = { false, true,  "VBAT SRAM retained; VBAT-domain wakeup; no return", NULL },
    [APP_LP_DPD2]  = { false, true,  "VBAT SRAM lost; VBAT-domain wakeup; no return",     NULL },
};

/*******************************************************************************
 * Wakeup source codes
 ******************************************************************************/
/*
 * One code per physical source.  Sync vs async edge detection is NOT enumerated
 * here: it is selected internally by the low-power mode (Sleep = synchronous;
 * Deep Sleep / Power Down = async / clock-less) - see spec section 4.2.
 */
#define APP_WAKEUP_UART             0U    /*!< HSP LPUART0 RX (Sleep: RX-full; DS: RX active edge). */
#define APP_WAKEUP_WAKE_GPIO        1U    /*!< WAKE GPIO / SW6 (PIO1_0); Sleep, DS 1/2/3, PD.       */
#define APP_WAKEUP_AON_GPIO         2U    /*!< AON GPIO / SW5 (PIO0_4); Sleep, DS 1/2/3, PD, DPD.   */
#define APP_WAKEUP_LPTMR            3U    /*!< VBAT LPTMR; Sleep, DS 1/2/3, PD, DPD.                */
#define APP_WAKEUP_RTC              4U    /*!< VBAT RTC alarm; Sleep, DS 1/2/3, PD, DPD (VBAT domain). */
#define APP_WAKEUP_DMA_WAKE         5U    /*!< WAKE eDMA3 CH0 m2m; completion wakes. Sleep, DS, PD. */
#define APP_WAKEUP_DMA_SW5          6U    /*!< WAKE eDMA3 CH0 m2m runs; SW5 wakes.   Sleep, DS, PD. */
#define APP_WAKEUP_NONE             0xFFU /*!< No wakeup source (DPD auto-wake, or back/cancel).    */

/*!
 * @brief Descriptor for one wakeup source, indexed by APP_WAKEUP_* code.
 *
 * Single source of truth for a source's static data plus its control function.
 * APP_SetWakeupSource() indexes the APP_WAKEUP_SOURCES[] table below: it runs
 * configure() for the peripheral-specific bits and does the generic
 * EnableIRQ(irq) + POWER_Enable/DisableWakeupSource(powerSrc) itself.
 */
typedef struct
{
    const char           *name;     /*!< Display name (log line + Level 2 menu).   */
    IRQn_Type             irq;      /*!< NVIC IRQ for this source.                 */
    power_wakeup_source_t powerSrc; /*!< Encoded POWERCON wakeup source.           */
    void (*configure)(bool on);     /*!< Peripheral-specific arm(true)/disarm, or NULL if none. */
    uint8_t               modes;    /*!< Supported LP modes: bitmask of APP_M(APP_LP_*). */
} app_wakeup_source_t;

/*
 * Per-source control functions are file-static in power_mode_switch.c; they are
 * forward-declared here so the descriptor table can live in this header.  (This
 * header is included by power_mode_switch.c only.)  DMA needs no peripheral-
 * specific config, so its descriptor .configure is NULL.
 */
static void APP_CfgUart(bool on);
static void APP_CfgWakeGpio(bool on);
static void APP_CfgAonGpio(bool on);
static void APP_CfgLptmr(bool on);
static void APP_CfgRtc(bool on);
static void APP_CfgDma(bool on);
static void APP_CfgDmaSw5(bool on);


/*!
 * Per-source wakeup descriptor table, indexed by APP_WAKEUP_* code.  Single
 * source of truth: display name, NVIC IRQ, encoded POWERCON wakeup source, the
 * peripheral configure() function, and the supported-LP-mode bitmask.  The two
 * DMA rows drive one WAKE_EDMA3 CH0 m2m transfer paced by WAKE_LPTMR_0; they
 * differ only in the armed wakeup (eDMA completion vs SW5).
 */
static const app_wakeup_source_t APP_WAKEUP_SOURCES[] = {
    [APP_WAKEUP_UART]      = { "UART RX",         APP_UART_IRQ,       kPOWER_WakeupIrq_HspLpuart0,  APP_CfgUart,
                              APP_M(APP_LP_SLEEP) | APP_M_DS },
    [APP_WAKEUP_WAKE_GPIO] = { "WAKE GPIO / SW6", APP_WAKE_GPIO_IRQ,  kPOWER_WakeupIrq_WakeGpioCh0, APP_CfgWakeGpio,
                              APP_M(APP_LP_SLEEP) | APP_M_DS | APP_M(APP_LP_PD) },
    [APP_WAKEUP_AON_GPIO]  = { "AON GPIO / SW5",  APP_AON_GPIO_IRQ,   kPOWER_WakeupIrq_VbatGpioCh0, APP_CfgAonGpio,
                              APP_M(APP_LP_SLEEP) | APP_M_DS | APP_M(APP_LP_PD) | APP_M_DPD },
    [APP_WAKEUP_LPTMR]     = { "VBAT LPTMR",      APP_VBAT_LPTMR_IRQ, kPOWER_WakeupIrq_VbatLptmr,   APP_CfgLptmr,
                              APP_M(APP_LP_SLEEP) | APP_M_DS | APP_M(APP_LP_PD) | APP_M_DPD },
    [APP_WAKEUP_RTC]       = { "RTC alarm",       APP_RTC_IRQ,        kPOWER_WakeupIrq_VbatRtc,     APP_CfgRtc,
                              APP_M(APP_LP_SLEEP) | APP_M_DS | APP_M(APP_LP_PD) | APP_M_DPD },
    /* DMA (wake on complete): eDMA CH0 completion is the armed wakeup. */
    [APP_WAKEUP_DMA_WAKE]  = { "DMA (wake on complete)", APP_WAKE_EDMA_IRQ, kPOWER_WakeupWakeDma_Lptmr0, APP_CfgDma,
                              APP_M(APP_LP_SLEEP) | APP_M_DS | APP_M(APP_LP_PD) },
    /* DMA (run, then SW5): same transfer, but SW5 / AON GPIO is the armed wakeup. */
    [APP_WAKEUP_DMA_SW5]   = { "DMA (run, then SW5)", APP_AON_GPIO_IRQ, kPOWER_WakeupIrq_VbatGpioCh0, APP_CfgDmaSw5,
                              APP_M(APP_LP_SLEEP) | APP_M_DS | APP_M(APP_LP_PD) },
};
#define APP_WAKEUP_SOURCE_COUNT \
    ((uint8_t)(sizeof(APP_WAKEUP_SOURCES) / sizeof(APP_WAKEUP_SOURCES[0])))

/*******************************************************************************
 * Run mode display names
 *
 * The demo uses the fsl_power driver's power_run_mode_t directly (queried via
 * POWER_GetCurrentRunMode()); it does not define its own run-mode enum.
 ******************************************************************************/
/*! Run-mode display names for the transition log, indexed by power_run_mode_t. */
static const char *const APP_RUN_MODE_NAMES[] = {
    [kPOWER_RunModeHp]     = "HP Run",
    [kPOWER_RunModeNormal] = "Normal Run",
    [kPOWER_RunModeLp]     = "Low Power Run",
};

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
    uint8_t     payload;   /*!< Run: power_run_mode_t; LP: APP_LP_* code.  */
} app_target_t;

/*******************************************************************************
 * Flat Top-level Target Table
 *
 * Ordered to match app_target_mode_t so APP_TARGETS[target] is always valid.
 * Targets <= kAPP_TargetLpRun are Run targets (switch run mode immediately);
 * the rest are low-power targets handled by APP_EnterLowPower().
 ******************************************************************************/
static const app_target_t APP_TARGETS[] = {
    [kAPP_TargetNormalRun] = { "Normal Run             (NP - 0.8 V/FBB, 800 MHz)",          "Normal Run",          kPOWER_RunModeNormal },
    [kAPP_TargetHpRun]     = { "HP Run                 (HP - 0.9 V/FBB, 1 GHz)",            "HP Run",              kPOWER_RunModeHp     },
    [kAPP_TargetLpRun]     = { "Low Performance Run    (LP - 0.8 V/ZBB, 600 MHz)",          "Low Performance Run", kPOWER_RunModeLp     },
    [kAPP_TargetSleep]     = { "Sleep",                                                     "Sleep",               APP_LP_SLEEP         },
    [kAPP_TargetDs1]       = { "Deep Sleep 1           (all domains retained)",             "Deep Sleep 1",        APP_LP_DS1           },
    [kAPP_TargetDs2]       = { "Deep Sleep 2           (CPU retained; NPU/COMM/MEDIA off)", "Deep Sleep 2",        APP_LP_DS2           },
    [kAPP_TargetDs3]       = { "Deep Sleep 3           (all domains off)",                  "Deep Sleep 3",        APP_LP_DS3           },
    [kAPP_TargetPowerDown] = { "Power Down             *** system resets on wakeup ***",    "Power Down",          APP_LP_PD            },
    [kAPP_TargetDpd1]      = { "Deep Power Down 1      (VBAT SRAM retained)",               "Deep Power Down 1",   APP_LP_DPD1          },
    [kAPP_TargetDpd2]      = { "Deep Power Down 2      (VBAT SRAM lost)",                   "Deep Power Down 2",   APP_LP_DPD2          },
};
#define APP_TARGET_COUNT ((uint8_t)(sizeof(APP_TARGETS) / sizeof(APP_TARGETS[0])))

/*******************************************************************************
 * Per-mode power configuration is built at runtime from g_powerModeTable below.
 * See APP_BuildSleepConfig(), APP_BuildDeepSleepConfig(), APP_BuildPowerDownConfig(),
 * and APP_BuildDeepPowerDownConfig() in power_mode_switch.c.
 ******************************************************************************/

/*******************************************************************************
 * LPCG mode type and active-mode load-test table *
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
 * RT2660 reference topology: CMC0 = COMPUTE/MAIN, CMC1 = WAKE, CMC2 = COMM.
 * Default reference value: APP_HSKSEL(1,1,1) = 0x7U (all three CMCs).
 */
#define APP_HSKSEL(cmc0, cmc1, cmc2) \
    ((uint8_t)(((cmc0) ? 1U : 0U) | ((cmc1) ? 2U : 0U) | ((cmc2) ? 4U : 0U)))

/*! @brief One entry in the application LPCG configuration table. */
typedef struct
{
    clock_ip_name_t lpcg;            /*!< LPCG identifier (clock_lpcg_t enum in fsl_clock.h). */
    app_lpcg_mode_t mode;            /*!< Target LPCG_CFG value (CCM_SLICE_CONTROL bits[1:0]). */
    uint8_t         hskSel;          /*!< HSK_SEL: CCM_SLICE_CONTROL bits[18:16].
                                      *   Built with APP_HSKSEL(cmc0,cmc1,cmc2).
                                      *   Selects which CMC instance(s) drive the clock-gate
                                      *   handshake for this LPCG during low-power transitions.
                                      *   Must match the routing in power_topology_config_t. */
    bool            bypassHandshake; /*!< If true, sets CCM_SLICE_CONTROL_HSK_BYPASS (bit 8) to bypass
                                      *   the CMC clock-gate handshake for this LPCG.
                                      *   Set to true ONLY for early bring-up when the downstream CMC
                                      *   handshake partner is not yet functional.  Default: false. */
} app_lpcg_config_t;

/*!
 * @brief Peripheral LPCG table for active-mode load test (REQ-008).
 *
 * Lists every peripheral LPCG across all RT2660 subsystems.  All entries
 * default to kAPP_LpcgModeOn (LPCG_CFG = 3: clocks ON, gated in DeepSleep).
 * Adjust individual entries to kAPP_LpcgModeOffInSleep (1) or
 * kAPP_LpcgModeOffInDeepSleep (2) for fine-grained power measurements.
 *
 * Entries marked "always ON" are system-critical clocks that enabling is
 * idempotent and safe; do NOT change their mode to kAPP_LpcgModeOff (0).
 *
 * hskSel (third field): CCM_SLICE_CONTROL HSK_SEL value (bits[18:16]),
 * built with APP_HSKSEL(cmc0,cmc1,cmc2).  Must match the CMC routing
 * configured in power_topology_config_t.  All entries below use
 * APP_HSKSEL(1,1,1) (= 0x7, all three CMCs - reference topology).
 * Update individual entries when using a non-default topology.
 *
 * bypassHandshake (fourth field): set to true to also write
 * CCM_SLICE_CONTROL_HSK_BYPASS (bit 8), bypassing the clock-gate handshake.
 * All entries use false (real CMC clock-gate handshake) EXCEPT
 * kCLOCK_CMPT_romcp and kCLOCK_CMPT_cm85_dbgclk, which keep true (bypass) for
 * bring-up.  Set an entry to true only when its downstream CMC handshake
 * partner is not yet functional.
 *
 * Terminated by a sentinel with lpcg == kCLOCK_IpInvalid.
 */
static const app_lpcg_config_t APP_LPCG_TABLE[] = {
    /* --- SYSCON subsystem [0] --- */
    { kCLOCK_SYSCON_freqme,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    /* --- CMPT subsystem [16..29] --- */
    { kCLOCK_CMPT_romcp,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), true },
    { kCLOCK_CMPT_cm85_clkin,           kAPP_LpcgModeOffInSleep,        APP_HSKSEL(1,0,0), false }, /* CPU clock input - always ON */
    { kCLOCK_CMPT_cm85_dbgclk,          kAPP_LpcgModeOffInSleep,        APP_HSKSEL(1,0,0), true },
    { kCLOCK_CMPT_cm85_iwic,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_CMPT_npu_core,             kAPP_LpcgModeOffInSleep,        APP_HSKSEL(1,0,0), false },
    { kCLOCK_CMPT_npu_mem,              kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_CMPT_freqme,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_CMPT_modcon,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_CMPT_trdc,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_CMPT_sram_ctrl0,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,1,1), false },//For DMA wake up
    { kCLOCK_CMPT_sram_ctrl1,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,1,1), false },//For DMA wake up
    { kCLOCK_CMPT_sram_ctrl2,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,1,1), false },//For DMA wake up
    { kCLOCK_CMPT_nic_gpv,              kAPP_LpcgModeOff,               APP_HSKSEL(1,1,1), false },
    { kCLOCK_CMPT_llc,                  kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    /* --- MAIN subsystem [32..106] --- */
    { kCLOCK_MAIN_trace_rt2660,         kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_tsclk,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_tpiu_traceclkin,      kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_swo_traceclkin,       kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_xspi0,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_xspi1,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_edma0,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_edma1,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_freqme,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_modcon,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_crc,              kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_dac,              kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_adc0,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_adc1,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_evtg0,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_evtg1,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_evtg2,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_evtg3,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_xbar0,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_xbar1,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_xbar2,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_sramc,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_trdc,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpspi0,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpspi1,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpspi2,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpspi3,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpspi4,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpi2c0,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpi2c1,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpuart0,          kAPP_LpcgModeOn,                APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpuart1,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpuart2,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpuart3,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpuart4,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpuart5,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_flexpwm0,         kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_flexpwm1,         kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_flexpwm2,         kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_flexpwm3,         kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpit0,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_lpit1,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_qtpm0,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_qtimer0,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_qtimer1,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_qtimer2,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_qtimer3,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_can0,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_can1,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_can2,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_qdc0,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_qdc1,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_qdc2,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_qdc3,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_rgpio0,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_rgpio1,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_rgpio2,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_rgpio3,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_rgpio4,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_i3c0,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_flexio0,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_flexio1,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_flexio2,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_sinc0,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_sinc1,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_mtr_master,           kAPP_LpcgModeOff,               APP_HSKSEL(1,1,1), false },
    { kCLOCK_MAIN_iomuxc,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_trig_sync0,       kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_trig_sync1,       kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_stm,              kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_hsp_nic_main_gpv,     kAPP_LpcgModeOff,               APP_HSKSEL(1,1,1), false },
    { kCLOCK_MAIN_prince0,              kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_prince1,              kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_cssi,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MAIN_mmu0,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    /* --- WAKE subsystem [112..144] --- */ //
    { kCLOCK_WAKE_syscon_pmu,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_WAKE_dap_rt2660,           kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false }, /* DAP/JTAG debug - always ON */
    { kCLOCK_WAKE_edma,                 kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_freqme,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_WAKE_modcon,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_WAKE_xbar0,                kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_trdc,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_WAKE_syscon_resetcon,      kAPP_LpcgModeOn,                APP_HSKSEL(1,0,0), false }, /* reset controller - always ON */
    { kCLOCK_WAKE_syscon_pdcon,         kAPP_LpcgModeOn,                APP_HSKSEL(1,0,0), false }, /* power domain controller - always ON */
    { kCLOCK_WAKE_syscon_powercon,      kAPP_LpcgModeOn,                APP_HSKSEL(1,0,0), false }, /* power sequencer - always ON */
    { kCLOCK_WAKE_syscon_memcon,        kAPP_LpcgModeOn,                APP_HSKSEL(1,0,0), false }, /* memory controller - always ON */
    { kCLOCK_WAKE_micfil,               kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_lpspi,                kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_lpi2c0,               kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_lpi2c1,               kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_lpuart0,              kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_lpuart1,              kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_acmp0,                kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_acmp1,                kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_acmp2,                kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_acmp3,                kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_swt0,                 kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_swt1,                 kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_lptimer0,             kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_lptimer1,             kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_qtpm,                 kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_ewm,                  kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_rgpio0,               kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_i3c0,                 kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_ram_ctrl,             kAPP_LpcgModeOn,                APP_HSKSEL(1,1,1), false },
    { kCLOCK_WAKE_iomuxc,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_WAKE_vbat_rtc_hp,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_WAKE_trig_sync,            kAPP_LpcgModeOn,                APP_HSKSEL(1,0,1), false },
    /* --- COMM subsystem [160..172] --- */
    { kCLOCK_COMM_xspir,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_COMM_eth0,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_COMM_eth1,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_COMM_usdhc0,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_COMM_usdhc1,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_COMM_usb0,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_COMM_usb1,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_COMM_xenophy0,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_COMM_xenophy1,             kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_COMM_freqme,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_COMM_modcon,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_COMM_trdc,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_COMM_nic_gpv,              kAPP_LpcgModeOff,               APP_HSKSEL(1,1,1), false },
    /* --- AUDIO subsystem [176..188] --- */    
    { kCLOCK_AUDIO_edma,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,1,0), false },
    { kCLOCK_AUDIO_freqme,              kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_AUDIO_modcon,              kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_AUDIO_xbar0,               kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_AUDIO_trdc,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_AUDIO_sai0,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_AUDIO_sai1,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_AUDIO_sai2,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_AUDIO_micfil,              kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_AUDIO_asrc,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_AUDIO_spdif_xcvr,          kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_AUDIO_mqs,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_AUDIO_ram_ctrl,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    /* --- MEDIA subsystem [192..203] --- */    
    { kCLOCK_MEDIA_jpeg_decoder,        kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MEDIA_isi,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MEDIA_reformatter,         kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MEDIA_csi,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MEDIA_mipi_csi,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MEDIA_gpu,                 kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MEDIA_dcif,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MEDIA_mipi_dsi,            kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MEDIA_freqme,              kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MEDIA_modcon,              kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MEDIA_trdc,                kAPP_LpcgModeOffInDeepSleep,    APP_HSKSEL(1,0,0), false },
    { kCLOCK_MEDIA_nic_gpv,             kAPP_LpcgModeOff,               APP_HSKSEL(1,1,1), false },
    /* --- sentinel --- */
    { kCLOCK_IpInvalid,                 kAPP_LpcgModeOff,               APP_HSKSEL(1,1,1), false },
};

/*! @} */

/*!
 * @defgroup app_power_init Power init configuration
 * @{
 *
 * Module-scope power init and topology config instances.
 *
 * Both are defined here as literal designated initializers - the values
 * mirror POWER_GetDefaultInitConfig() / POWER_GetDefaultTopologyConfig()
 * (POR-default behavior) but are exposed in one editable location so that
 * a board / use case can tune fields directly without overriding at run
 * time inside main().
 *
 * To customise topology for a non-reference board design, edit the
 * matching field of @ref s_topologyCfg below.  Remember to update the
 * hskSel field of every affected APP_LPCG_TABLE entry so that
 * CCM SLICE_CONTROL.HSK_SEL stays in sync.
 *
 * CMC / SSC step-mode defaults (kPOWER_StepModeHandshake, countValue = 0)
 * mirror the hardware reset state; change them per step if a build needs
 * Count-mode timing instead of Handshake.
 */

/*! POR-default per-step config used by every CMC and SSC step below. */
#define APP_POWER_STEP_DEFAULT                       \
    {                                                \
        .sleepMode  = kPOWER_StepModeHandshake,      \
        .wakeupMode = kPOWER_StepModeHandshake,      \
        .countValue = 0U,                            \
    }

/*! POR-default 5-step config used by every CMC instance below. */
#define APP_POWER_CMC_STEP_DEFAULT                   \
    {                                                \
        .busMasterLpcg = APP_POWER_STEP_DEFAULT,     \
        .busSlaveLpcg  = APP_POWER_STEP_DEFAULT,     \
        .rootClockGate = APP_POWER_STEP_DEFAULT,     \
        .clockSource   = APP_POWER_STEP_DEFAULT,     \
        .powerDomain   = APP_POWER_STEP_DEFAULT,     \
    }

/*!
 * @brief SoC topology - HSK_SEL routing and PDCON per-domain handshake mask.
 *
 * rcgcfgHskSel / csrccfgHskSel / csrccfgHskSel1 = 0x11111111 / 0x11111111 /
 * 0x1 routes every root clock and clock source to CMC0 only (this example).
 * Use 0x77777777 / 0x77777777 / 0x7 to route to all three CMCs (CMC0 CPU,
 * CMC1 MAIN, CMC2 WAKE) - the RT2660 reference topology.
 *
 * domainHsk[] = kPDCON_HandshakeAll for all 6 domains means every PDCON
 * domain transition handshakes with all units.
 *
 * Edit any field below for a non-reference board.
 *
 * External linkage (not static): the definition lives in this header but is instantiated in the
 * single TU that includes it (power_mode_switch.c); BOARD_InitHardware() references it via the
 * `extern` declaration in the board app.h to run POWER_Init().
 */
power_topology_config_t s_topologyCfg = {
    .rcgcfgHskSel   = 0x11111111UL, /*!< Root clock -> CMC routing (all roots -> CMC0 only)        */
    .csrccfgHskSel  = 0x11111111UL, /*!< Clock source -> CMC routing word 0 (all sources -> CMC0)  */
    .csrccfgHskSel1 = 0x1UL,        /*!< Clock source -> CMC routing word 1 (FRO12M_LP -> CMC0)    */
    .domainHsk      = {
        kPDCON_HandshakeAll, /*!< [0] PD_CPU   handshake mask */
        kPDCON_HandshakeAll, /*!< [1] PD_NPU   handshake mask */
        kPDCON_HandshakeAll, /*!< [2] PD_COMM  handshake mask */
        kPDCON_HandshakeAll, /*!< [3] PD_MEDIA handshake mask */
        kPDCON_HandshakeAll, /*!< [4] PD_USB   handshake mask */
        kPDCON_HandshakeAll, /*!< [5] PD_MIPI  handshake mask */
    },
};

/*!
 * @brief Power init config passed to POWER_Init().
 *
 * cmcCpu / cmcMain / cmcWake use the all-Handshake default for the 5
 * CMC steps.  sscPmu / sscPmic likewise use Handshake for both the
 * sleep and wakeup directions.  Override individual step fields if a
 * build needs Count-mode timing on a specific CMC step.
 *
 * topology points at @ref s_topologyCfg above; replace with NULL to fall
 * back to driver-internal POR defaults (skipping any field edits above).
 *
 * External linkage (see s_topologyCfg note): defined here, instantiated in power_mode_switch.c,
 * consumed by BOARD_InitHardware() through the board app.h `extern` declaration.
 */
power_init_config_t s_powerInitCfg = {
    .cmcCpu   = APP_POWER_CMC_STEP_DEFAULT, /*!< CMC0 - CPU / M85 core (COMPUTE_SS)             */
    .cmcMain  = APP_POWER_CMC_STEP_DEFAULT, /*!< CMC1 - MAIN domain (eDMA3 + eDMA5 DMA wakeup)  */
    .cmcWake  = APP_POWER_CMC_STEP_DEFAULT, /*!< CMC2 - WAKE domain (eDMA3 DMA wakeup)          */
    .sscPmu   = APP_POWER_STEP_DEFAULT,     /*!< SSC STEP1 - PMU standby/exit via P-Channel     */
    .sscPmic  = APP_POWER_STEP_DEFAULT,     /*!< SSC STEP2 - PMIC standby/exit                  */
    .topology = &s_topologyCfg,             /*!< Apply topology literal above (NULL = POR HW)  */
};
/*! @} */

/*******************************************************************************
 * Resource x Mode configuration table
 *
 * 2-D reference table: rows = configurable resources (Y-axis),
 *                      columns = standby modes (X-axis).
 *
 * X-axis (4 columns): Sleep | DS1 | DS2 | DS3
 * Y-axis (rows):      every resource configurable in any standby mode.
 *
 * Cell encoding:
 *   _C(v)  - configurable; v is the typed value passed to the API struct field.
 *   _NC_   - NOT configurable / inapplicable; builder leaves the struct field at 0.
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
    kAPP_StandbyModeIdx_Sleep     = 0U, /*!< Sleep - wakeup source does not affect clock config. */
    kAPP_StandbyModeIdx_DS1       = 1U, /*!< Deep Sleep 1 - all domains retained. */
    kAPP_StandbyModeIdx_DS2       = 2U, /*!< Deep Sleep 2 - CPU retained, NPU/COMM/MEDIA off. */
    kAPP_StandbyModeIdx_DS3       = 3U, /*!< Deep Sleep 3 - all domains off. */
    /* Power Down / Deep Power Down are NOT table columns: their retention config is a
     * handful of scalars, inlined directly in APP_BuildPowerDownConfig() /
     * APP_BuildDeepPowerDownConfig() rather than driven by g_powerModeTable. */
    kAPP_StandbyModeIdx_COUNT     = 4U
} app_standby_mode_idx_t;

/*! Configurable resource row identifiers - Y-axis of g_powerModeTable. */
typedef enum _power_standby_resource
{
    /* Power domain events - PDCON.PDSLPCFG */
    kPOWER_Resource_CpuDomainEvent    = 0U,
    kPOWER_Resource_NpuDomainEvent    = 1U,
    kPOWER_Resource_CommDomainEvent   = 2U,
    kPOWER_Resource_MediaDomainEvent  = 3U,

    /* [EXPERIMENTAL] PMU analog state - POWERCON_SOC_CTRL.PMUCFG_STBY */
    kPOWER_Resource_PmuMode           = 4U,
    kPOWER_Resource_DcdcMode          = 5U,
    kPOWER_Resource_CoreLvl           = 6U,
    kPOWER_Resource_Ldo0V8Mode        = 7U,
    kPOWER_Resource_Ldo1V8Mode        = 8U,
    kPOWER_Resource_LdoVdda1V8Mode    = 9U,
    kPOWER_Resource_Hqref             = 10U,
    kPOWER_Resource_Sensors           = 11U,

    /* [EXPERIMENTAL] Clock source standby gating - POWERCON_SOC_CTRL.CSRCCFG_STBY */
    kPOWER_Resource_ClkLdoa0V8        = 12U,
    kPOWER_Resource_ClkFro192M        = 13U,
    kPOWER_Resource_ClkFro12M         = 14U,
    kPOWER_Resource_ClkMainPll        = 15U,
    kPOWER_Resource_ClkCorePll        = 16U,
    kPOWER_Resource_ClkSysPll         = 17U,
    kPOWER_Resource_ClkLdoq0V8        = 18U,
    kPOWER_Resource_ClkSxosc          = 19U,
    kPOWER_Resource_ClkFro12MLp       = 20U,

    /* [EXPERIMENTAL] Memory slice standby modes - MEMCON_SLICE.MEM_SLPCFG */
    kPOWER_Resource_WakeSram          = 21U,
    kPOWER_Resource_Ocram             = 22U,
    kPOWER_Resource_LlcCache          = 23U,
    kPOWER_Resource_Audio8Kb          = 24U,
    kPOWER_Resource_M85Tcm            = 25U,
    kPOWER_Resource_NpuTcm            = 26U,

    /* Power Down / Deep Power Down retention are NOT table resources: those scalars
     * (retainOcram0KB / retainPkcMem / retainVbatSram) are inlined in the PD/DPD builders. */

    /* [EXPERIMENTAL] Root clock standby enable - POWERCON_SOC_CTRL.RCGCFG_STBY */
    /* bit=1 means clock ENABLED in standby (active-high, opposite of CSRCCFG_STBY). */
    kPOWER_Resource_RcgCompute        = 27U, /*!< COMPUTE_SS root clock (bit 0) */
    kPOWER_Resource_RcgMain           = 28U, /*!< MAIN_SS root clock (bit 1) */
    kPOWER_Resource_RcgWake           = 29U, /*!< WAKE_SS root clock (bit 2) */
    kPOWER_Resource_RcgComm           = 30U, /*!< COMM_SS root clock (bit 3) */
    kPOWER_Resource_RcgMedia          = 31U, /*!< MEDIA_SS root clock (bit 4) */
    kPOWER_Resource_RcgAudio          = 32U, /*!< AUDIO_SS root clock (bit 5) */
    kPOWER_Resource_RcgWake1M         = 33U, /*!< WAKE_1M root clock (bit 6) */
    kPOWER_Resource_RcgWake2M         = 34U, /*!< WAKE_2M root clock (bit 7) */

    kPOWER_Resource_COUNT             = 35U
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
    bool     valid;   /*!< true = configurable in this mode; false = NOT_CONFIGURABLE. */
    uint32_t value;   /*!< Config value cast to uint32_t; 0 when !valid. */
    uint8_t  hskSel;  /*!< POWERCON HSK_SEL 3-bit field for this resource's CMC routing.
                       *   Built with APP_HSKSEL(cmc0,cmc1,cmc2).  Only meaningful for
                       *   kPOWER_Resource_Clk* (CSRCCFG) and kPOWER_Resource_Rcg*
                       *   (RCGCFG) rows; ignored for all other resource types.
                       *   Used by APP_ApplyTopologyForMode() to call POWER_SetTopology()
                       *   with the correct per-mode CMC routing before mode entry. */
} power_mode_cell_t;

/* Cell constructor macros - used only inside the table below; undefined after. */
#define _NC_      { false, 0U, 0U }               /*!< NOT_CONFIGURABLE. */
#define _C(v)     { true, (uint32_t)(v), 0U }     /*!< Configurable; no topology role (hskSel=0 = not a CSRCCFG/RCGCFG resource). */
#define _CT(v,h)  { true, (uint32_t)(v), (uint8_t)(h) } /*!< Configurable with explicit CMC routing via APP_HSKSEL(cmc0,cmc1,cmc2). */
                                                         /*!< Use _CT for all kPOWER_Resource_Clk* and kPOWER_Resource_Rcg* rows    */
                                                         /*!< so per-resource CMC routing is visible in the table.                  */
#define _KEEP_(CMC0, CMC1, CMC2) _CT(true,  APP_HSKSEL(CMC0, CMC1, CMC2)) /*!< Clock/root-clock: keep ENABLED in standby, routed to CMC(CMC0,CMC1,CMC2). */
#define _GATE_(CMC0, CMC1, CMC2) _CT(false, APP_HSKSEL(CMC0, CMC1, CMC2)) /*!< Clock/root-clock: gate OFF  in standby, routed to CMC(CMC0,CMC1,CMC2). */

/* Short value aliases - keeps table cells narrow. */
#define EVT_ACT   kPDCON_EventNoneOrActive   /*!< Domain: stays On (active).         */
                                             /*!< RT2660 has no PDCON domain         */
                                             /*!< retention (omitted for die size),  */
                                             /*!< so domains are only ever On or Off.*/
#define EVT_OFF   kPDCON_EventPowerOff       /*!< Domain: powered off.               */
#define DCDC_PWM  kPMU_DcdcModePWM           /*!< DCDC: PWM (higher power, lower noise). */
#define DCDC_PFM  kPMU_DcdcModePFM           /*!< DCDC: PFM (lower power).           */
#define LDO_HP    kPMU_LdoModeHP             /*!< LDO: high-performance mode.        */
#define LDO_LP    kPMU_LdoModeLP             /*!< LDO: low-power mode.               */
#define MEM_ACT   kMEMCON_PowerModeActive    /*!< SRAM: fully active.                */
#define MEM_RET   kMEMCON_PowerModeRetention /*!< SRAM: data retained, power reduced. */
#define MEM_PD    kMEMCON_PowerModePowerDown /*!< SRAM: powered down, data lost.     */

/*! Human-readable resource row names (indexed by power_standby_resource_t). */
static const char *const g_powerResourceNames[kPOWER_Resource_COUNT] = {
    [kPOWER_Resource_CpuDomainEvent]    = "CPU domain event",
    [kPOWER_Resource_NpuDomainEvent]    = "NPU domain event",
    [kPOWER_Resource_CommDomainEvent]   = "COMM domain event",
    [kPOWER_Resource_MediaDomainEvent]  = "MEDIA domain event",
    [kPOWER_Resource_PmuMode]           = "[EXP] PMU mode",
    [kPOWER_Resource_DcdcMode]          = "[EXP] DCDC mode",
    [kPOWER_Resource_CoreLvl]           = "[EXP] Core voltage level",
    [kPOWER_Resource_Ldo0V8Mode]        = "[EXP] LDO_0V8 mode",
    [kPOWER_Resource_Ldo1V8Mode]        = "[EXP] LDO_1V8 mode",
    [kPOWER_Resource_LdoVdda1V8Mode]    = "[EXP] LDO_VDDA_1V8 mode",
    [kPOWER_Resource_Hqref]             = "[EXP] HQREF enable",
    [kPOWER_Resource_Sensors]           = "[EXP] Sensor enable mask",
    [kPOWER_Resource_ClkLdoa0V8]        = "[EXP] LDOA_0V8 keep-on",
    [kPOWER_Resource_ClkFro192M]        = "[EXP] FRO_192M keep-on",
    [kPOWER_Resource_ClkFro12M]         = "[EXP] FRO_12M keep-on",
    [kPOWER_Resource_ClkMainPll]        = "[EXP] MAINPLL keep-on",
    [kPOWER_Resource_ClkCorePll]        = "[EXP] COREPLL keep-on",
    [kPOWER_Resource_ClkSysPll]         = "[EXP] SYSPLL keep-on",
    [kPOWER_Resource_ClkLdoq0V8]        = "[EXP] LDOQ_0V8 keep-on",
    [kPOWER_Resource_ClkSxosc]          = "[EXP] SXOSC keep-on",
    [kPOWER_Resource_ClkFro12MLp]       = "[EXP] FRO_12M_LP keep-on",
    [kPOWER_Resource_WakeSram]          = "[EXP] WAKE SRAM mode",
    [kPOWER_Resource_Ocram]             = "[EXP] OCRAM mode",
    [kPOWER_Resource_LlcCache]          = "[EXP] LLC cache mode",
    [kPOWER_Resource_Audio8Kb]          = "[EXP] Audio 8KB mode",
    [kPOWER_Resource_M85Tcm]            = "[EXP] M85 TCM mode",
    [kPOWER_Resource_NpuTcm]            = "[EXP] NPU TCM mode",
    [kPOWER_Resource_RcgCompute]        = "[EXP] COMPUTE root clk on",
    [kPOWER_Resource_RcgMain]           = "[EXP] MAIN root clk on",
    [kPOWER_Resource_RcgWake]           = "[EXP] WAKE root clk on",
    [kPOWER_Resource_RcgComm]           = "[EXP] COMM root clk on",
    [kPOWER_Resource_RcgMedia]          = "[EXP] MEDIA root clk on",
    [kPOWER_Resource_RcgAudio]          = "[EXP] AUDIO root clk on",
    [kPOWER_Resource_RcgWake1M]         = "[EXP] WAKE_1M root clk on",
    [kPOWER_Resource_RcgWake2M]         = "[EXP] WAKE_2M root clk on",
};

/*! Standby mode column header names (indexed by app_standby_mode_idx_t). */
static const char *const g_standbyModeNames[kAPP_StandbyModeIdx_COUNT] = {
    [kAPP_StandbyModeIdx_Sleep] = "Sleep",
    [kAPP_StandbyModeIdx_DS1]   = "DS1",
    [kAPP_StandbyModeIdx_DS2]   = "DS2",
    [kAPP_StandbyModeIdx_DS3]   = "DS3",
};

/*!
 * @brief Resource x Mode 2-D configuration table.
 *
 * Index: g_powerModeTable[resource][mode_idx]
 *
 * Cell macros (defined above, #undef'd after the table):
 *   _NC_    = NOT_CONFIGURABLE in this mode; builder skips the field.
 *   _C(v)   = configurable; v is the API enum/value written to the struct field.
 *   _KEEP_(c0,c1,c2) = CSRCCFG/RCGCFG resource: keep ENABLED in standby; CMC routing APP_HSKSEL(c0,c1,c2).
 *   _GATE_(c0,c1,c2) = CSRCCFG/RCGCFG resource: gate OFF in standby; CMC routing APP_HSKSEL(c0,c1,c2).
 *
 * Column index (X-axis) — Sleep + Deep Sleep variants only:
 *   [0] Sleep | [1] DS1 | [2] DS2 | [3] DS3
 *
 * Power Down / Deep Power Down are NOT columns here: their retention config is inlined
 * in APP_BuildPowerDownConfig() / APP_BuildDeepPowerDownConfig().
 *
 * DMA wakeup reuses the DS1/DS2/DS3 base columns and applies the historical DMA-only
 * clock-source overrides in code after the table-driven build.
 */
static const power_mode_cell_t
    g_powerModeTable[kPOWER_Resource_COUNT][kAPP_StandbyModeIdx_COUNT] =
{
    /*  resource                      [0]Sleep      [1]DS1       [2]DS2       [3]DS3       */
    /* ---- Domain events (PDCON.PDSLPCFG) ------------------------------------------------------------------------------------------------ */
    [kPOWER_Resource_CpuDomainEvent]   = { _C(EVT_ACT), _C(EVT_ACT), _C(EVT_ACT), _C(EVT_OFF) },
    [kPOWER_Resource_NpuDomainEvent]   = { _C(EVT_ACT), _C(EVT_ACT), _C(EVT_OFF), _C(EVT_OFF) },
    [kPOWER_Resource_CommDomainEvent]  = { _C(EVT_ACT), _C(EVT_ACT), _C(EVT_OFF), _C(EVT_OFF) },
    [kPOWER_Resource_MediaDomainEvent] = { _C(EVT_ACT), _C(EVT_ACT), _C(EVT_OFF), _C(EVT_OFF) },
    /* ---- PMU analog (PMUCFG_STBY) [EXPERIMENTAL] --------------------------------------------------------------------------------------- */
    /* Sleep: SSC NOT triggered; values written but no analog transition.                                                                      */
    /* DeepSleep: SSC IS triggered; values drive actual hardware transitions.                                                                  */
    /* TODO(V0.9, pending silicon): Deep Sleep targets are PmuMode=LP, CoreLvl=0.65V, Ldo1V8/LdoVdda1V8=LP (spec section 2.3).                  */
    /*   Kept at active-mode/HP placeholders below until silicon characterization confirms the codes; flip the DS columns then.               */
    [kPOWER_Resource_PmuMode]          = { _C(0U),       _C(2U),       _C(2U),       _C(2U) }, /* 0=HP (TODO V0.9: DS=LP) */
    [kPOWER_Resource_DcdcMode]         = { _C(DCDC_PWM), _C(DCDC_PWM), _C(DCDC_PFM), _C(DCDC_PFM) },
    [kPOWER_Resource_CoreLvl]          = { _C(0U),       _C(0x11U),    _C(0x11U),    _C(0x11U) }, /* 0=use active-mode level */
    [kPOWER_Resource_Ldo0V8Mode]       = { _C(LDO_HP),   _C(LDO_HP),   _C(LDO_LP),   _C(LDO_LP) },
    [kPOWER_Resource_Ldo1V8Mode]       = { _C(LDO_HP),   _C(LDO_HP),   _C(LDO_LP),   _C(LDO_LP) },
    [kPOWER_Resource_LdoVdda1V8Mode]   = { _C(1U),       _C(1U),       _C(2U),       _C(2U) }, /* 1=HP */
    [kPOWER_Resource_Hqref]            = { _C(true),     _C(true),     _C(true),     _C(true) }, /* V0.9: HQREF ON in Sleep/DeepSleep; off only in PMU-RET (Power Down) */
    [kPOWER_Resource_Sensors]          = { _C(0U),       _C(0U),       _C(0U),       _C(0U) }, /* all sensors off */
    /* ---- Clock source gating (CSRCCFG_STBY + CSRCCFG_HSK_SEL) [EXPERIMENTAL] ---------------------------------- */
    /* _KEEP_(c0,c1,c2)=retain enabled; _GATE_(c0,c1,c2)=cut off.  Set per-cell CMC routing directly: (1,0,0)=CMC0  */
    /* only (this example); e.g. _KEEP_(1,0,0) would route a resource to all three CMCs.                            */
    /* DMA wakeup keeps its historical clock-source differences through runtime overrides. */
    /*                               [0]Slp          [1]DS1        [2]DS2        [3]DS3 */
    [kPOWER_Resource_ClkLdoa0V8]  = { _KEEP_(1,0,0), _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    [kPOWER_Resource_ClkFro192M]  = { _KEEP_(1,0,0), _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    [kPOWER_Resource_ClkFro12M]   = { _KEEP_(1,0,0), _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    [kPOWER_Resource_ClkMainPll]  = { _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) }, /* DIV4 auto-restored on wakeup */
    [kPOWER_Resource_ClkCorePll]  = { _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    [kPOWER_Resource_ClkSysPll]   = { _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    [kPOWER_Resource_ClkLdoq0V8]  = { _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    [kPOWER_Resource_ClkSxosc]    = { _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    [kPOWER_Resource_ClkFro12MLp] = { _KEEP_(1,0,0), _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    /* ---- Memory slice modes (MEMCON_SLICE.MEM_SLPCFG) [EXPERIMENTAL] -------------------------------------------------------------------- */
    /* Sleep: Active.  DeepSleep: Retention, EXCEPT a domain's memory is PowerDown (MEM_PD) once its power domain is gated off              */
    /* (NPU/COMM/MEDIA TCM from DS2; M85 TCM in DS3) -- an unpowered domain cannot retain.  Always-on OCRAM-slice memory                     */
    /* (Ocram/Audio8Kb/LlcCache) and WAKE SRAM stay Retention across all DS variants.                                                        */
    [kPOWER_Resource_WakeSram]         = { _C(MEM_ACT), _C(MEM_RET), _C(MEM_RET), _C(MEM_RET) },
    [kPOWER_Resource_Ocram]            = { _C(MEM_ACT), _C(MEM_RET), _C(MEM_RET), _C(MEM_RET) },
    [kPOWER_Resource_LlcCache]         = { _C(MEM_ACT), _C(MEM_RET), _C(MEM_RET), _C(MEM_RET) },
    [kPOWER_Resource_Audio8Kb]         = { _C(MEM_ACT), _C(MEM_RET), _C(MEM_RET), _C(MEM_RET) },
    [kPOWER_Resource_M85Tcm]           = { _C(MEM_ACT), _C(MEM_RET), _C(MEM_RET), _C(MEM_PD) }, /* CPU off in DS3 -> PowerDown */
    [kPOWER_Resource_NpuTcm]           = { _C(MEM_ACT), _C(MEM_RET), _C(MEM_PD),  _C(MEM_PD) }, /* NPU off from DS2 -> PowerDown */
    /* ---- Root clock standby enable (RCGCFG_STBY + RCGCFG_HSK_SEL) [EXPERIMENTAL] ----------------------------------------- */
    /* Active-high register: _KEEP_(c0,c1,c2)=bit=1 (enabled); _GATE_(c0,c1,c2)=bit=0 (gated).  Opposite polarity to CSRCCFG_STBY. */
    /* DeepSleep: gate all root clocks except WAKE_1M/2M. */
    /*                               [0]Slp          [1]DS1        [2]DS2        [3]DS3 */
    [kPOWER_Resource_RcgCompute]  = { _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    [kPOWER_Resource_RcgMain]     = { _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    [kPOWER_Resource_RcgWake]     = { _KEEP_(1,0,0), _KEEP_(1,0,0), _KEEP_(1,0,0), _KEEP_(1,0,0) }, /* WAKE_SS stays on */
    [kPOWER_Resource_RcgComm]     = { _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    [kPOWER_Resource_RcgMedia]    = { _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    [kPOWER_Resource_RcgAudio]    = { _KEEP_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0), _GATE_(1,0,0) },
    [kPOWER_Resource_RcgWake1M]   = { _KEEP_(1,0,0), _KEEP_(1,0,0), _KEEP_(1,0,0), _KEEP_(1,0,0) }, /* always on: WAKE 1M timer */
    [kPOWER_Resource_RcgWake2M]   = { _KEEP_(1,0,0), _KEEP_(1,0,0), _KEEP_(1,0,0), _KEEP_(1,0,0) }, /* always on: WAKE 2M timer */
};

/* Remove table-local macros from the preprocessor namespace. */
#undef _NC_
#undef _C
#undef _CT
#undef _KEEP_
#undef _GATE_
#undef EVT_ACT
#undef EVT_OFF
#undef DCDC_PWM
#undef DCDC_PFM
#undef LDO_HP
#undef LDO_LP
#undef MEM_ACT
#undef MEM_RET
#undef MEM_PD

#endif /* _POWER_MODE_SWITCH_H_ */
