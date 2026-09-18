/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "power_mode_switch.h"

/*!
 * @addtogroup power_mode_switch_rt2660
 * @{
 *
 * SoC-level reference data that drives actual low-power hardware transitions:
 * the resource x mode table, the per-mode descriptor table, the LPCG
 * load-test table, and their field-setter maps. None of this depends on board
 * wiring (pins/IRQs) - that lives in the board's app.h / hardware_init.c
 * instead. See power_mode_switch.h for the types/macros used below and for
 * what each table is consumed by. Debug-only dumps of this data (not data
 * that drives behavior) live in power_mode_switch_debug.c.
 */

/*******************************************************************************
 * Run mode display names
 ******************************************************************************/
const char *const APP_RUN_MODE_NAMES[APP_RUN_MODE_NAME_COUNT] = {
    [kPOWER_RunModeHp]     = "HP Run",
    [kPOWER_RunModeNormal] = "Normal Run",
    [kPOWER_RunModeLp]     = "Low Power Run",
};

/*******************************************************************************
 * Flat Top-level Target Table
 *
 * Ordered to match app_target_mode_t so APP_TARGETS[target] is always valid.
 * Targets <= kAPP_TargetLpRun are Run targets (switch run mode immediately);
 * the rest are low-power targets handled by APP_EnterLowPower().
 ******************************************************************************/
const app_target_t APP_TARGETS[kAPP_TargetCount] = {
    [kAPP_TargetNormalRun] = { "Normal Run             (NP - 0.8 V/FBB, 800 MHz)",          "Normal Run",          kPOWER_RunModeNormal },
    [kAPP_TargetHpRun]     = { "HP Run                 (HP - 0.9 V/FBB, 1 GHz)",            "HP Run",              kPOWER_RunModeHp     },
    [kAPP_TargetLpRun]     = { "Low Performance Run    (LP - 0.8 V/ZBB, 600 MHz)",          "Low Performance Run", kPOWER_RunModeLp     },
    [kAPP_TargetSleep]     = { "Sleep",                                                     "Sleep",               APP_LP_SLEEP         },
    [kAPP_TargetDs1]       = { "Deep Sleep 1           (all domains retained)",             "Deep Sleep 1",        APP_LP_DS1           },
    [kAPP_TargetDs2]       = { "Deep Sleep 2           (CPU retained; NPU/COMM/MEDIA off)", "Deep Sleep 2",        APP_LP_DS2           },
    [kAPP_TargetDs3]       = { "Deep Sleep 3           (all domains off)",                  "Deep Sleep 3",        APP_LP_DS3           },
    [kAPP_TargetPowerDown] = { "Power Down             (DCDC OFF)",                         "Power Down",          APP_LP_PD            },
    [kAPP_TargetDpd1]      = { "Deep Power Down 1      (VBAT SRAM retained)",               "Deep Power Down 1",   APP_LP_DPD1          },
    [kAPP_TargetDpd2]      = { "Deep Power Down 2      (VBAT SRAM lost)",                   "Deep Power Down 2",   APP_LP_DPD2          },
};

/*!
 * @brief Peripheral LPCG table for active-mode load test.
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
 * configured in power_handshake_routing_config_t.  All entries below use
 * APP_HSKSEL(1,1,1) (= 0x7, all three CMCs - reference handshake routing).
 * Update individual entries when using non-default handshake routing.
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
const app_lpcg_config_t APP_LPCG_TABLE[] = {
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

/*******************************************************************************
 * g_powerModeTable - cell constructor macros and value aliases
 ******************************************************************************/

/* Cell constructor macros - used only inside g_powerModeTable below; undefined after. */
#define _NC_      { false, 0U, 0U }               /*!< NOT_CONFIGURABLE. */
#define _C(v)     { true, (uint32_t)(v), 0U }     /*!< Configurable; no handshake-routing role (hskSel=0 = not a CSRCCFG/RCGCFG resource). */
#define _CT(v,h)  { true, (uint32_t)(v), (uint8_t)(h) } /*!< Configurable with explicit CMC routing via APP_HSKSEL(cmc0,cmc1,cmc2). */
                                                         /*!< Use _CT for all kPOWER_Resource_Clk* and kPOWER_Resource_Rcg* rows    */
                                                         /*!< so per-resource CMC routing is visible in the table.                  */

/*
 * ON / OFF - unified on/off cell macros for every resource with on/off semantics.
 *
 *   ON()             / OFF()             -> domain-event rows (no CMC routing):
 *                                           enabled = domain stays On (active) / powered Off.
 *   ON(c0,c1,c2)     / OFF(c0,c1,c2)     -> clock / root-clock rows (CSRCCFG/RCGCFG):
 *                                           keep the resource ENABLED / gate it OFF in standby,
 *                                           routed to the given CMC(s) via APP_HSKSEL(c0,c1,c2).
 *
 * Both spellings dispatch on argument count: the 0-arg form builds a plain _C() cell
 * carrying the PDCON domain event; the 3-arg form builds a _CT() cell carrying the
 * standby keep/gate value plus the per-cell CMC handshake routing.
 */
#define _APP_ARG4(_0, _1, _2, _3, N, ...) N
#define _APP_NARG3(...) _APP_ARG4(__VA_ARGS__, 3, 2, 1, 0)
#define _APP_CAT(a, b)  _APP_CAT_(a, b)
#define _APP_CAT_(a, b) a##b

#define ON(...)  _APP_CAT(_ON_,  _APP_NARG3(dummy, ##__VA_ARGS__))(__VA_ARGS__)
#define OFF(...) _APP_CAT(_OFF_, _APP_NARG3(dummy, ##__VA_ARGS__))(__VA_ARGS__)

#define _ON_0()             _C(kPDCON_EventNoneOrActive)              /*!< Domain: stays On (active). */
#define _ON_3(c0, c1, c2)   _CT(true,  APP_HSKSEL(c0, c1, c2))        /*!< Clock/root-clock: keep ENABLED in standby, routed to CMC(c0,c1,c2). */
#define _OFF_0()            _C(kPDCON_EventPowerOff)                  /*!< Domain: powered off. */
#define _OFF_3(c0, c1, c2)  _CT(false, APP_HSKSEL(c0, c1, c2))       /*!< Clock/root-clock: gate OFF in standby, routed to CMC(c0,c1,c2). */

/*
 * RT2660 has no PDCON domain retention (omitted for die size), so domains are
 * only ever On (ON()) or Off (OFF()).
 */

/*!
 * @brief Resource x Mode 2-D configuration table.
 *
 * Index: g_powerModeTable[resource][mode_idx]
 *
 * Cell macros (defined above):
 *   _NC_    = NOT_CONFIGURABLE in this mode; builder skips the field.
 *   _C(v)   = configurable; v is the API enum/value written to the struct field.
 *   ON()  / OFF()            = domain-event rows: domain stays On (active) / powered Off.
 *   ON(c0,c1,c2) / OFF(c0,c1,c2) = CSRCCFG/RCGCFG resource: keep ENABLED / gate OFF in
 *                                  standby; CMC routing APP_HSKSEL(c0,c1,c2).
 *
 * Column index (X-axis): Sleep + Deep Sleep variants + Power Down:
 *   [0] Sleep | [1] DS1 | [2] DS2 | [3] DS3 | [4] PD
 *
 */
const power_mode_cell_t
    g_powerModeTable[kPOWER_Resource_COUNT][kAPP_StandbyModeIdx_COUNT] =
{
    /*  resource                      [0]Sleep      [1]DS1       [2]DS2       [3]DS3       [4]PD */
    /* ---- Domain events (PDCON.PDSLPCFG) ------------------------------------------------------------------------------------------------ */
    [kPOWER_Resource_CpuDomain]   = { ON(),  ON(),  ON(),  OFF(), _NC_ },
    [kPOWER_Resource_NpuDomain]   = { ON(),  ON(),  OFF(), OFF(), _NC_ },
    [kPOWER_Resource_CommDomain]  = { ON(),  ON(),  OFF(), OFF(), _NC_ },
    [kPOWER_Resource_MediaDomain] = { ON(),  ON(),  OFF(), OFF(), _NC_ },
    /* ---- PMU analog (PMUCFG_STBY) [EXPERIMENTAL] ---- */
    /* Sleep: SSC not triggered, values written but no analog transition. */
    /* Deep Sleep: SSC triggered, values drive actual hardware transitions. */
    /* PD column: _NC_ - PMU/DCDC/LDO state in Power Down is inlined by the driver. */
    /* Deep Sleep PmuMode/CoreLvl/Ldo1V8/LdoVdda1V8 are kept at active-mode/HP values
     * below pending silicon confirmation of the intended low-power targets. */
    [kPOWER_Resource_PmuMode]          = { _C(PMU_HP),   _C(PMU_LP),   _C(PMU_LP),   _C(PMU_LP),   _NC_ },
    [kPOWER_Resource_DcdcMode]         = { _C(DCDC_PWM), _C(DCDC_PWM), _C(DCDC_PFM), _C(DCDC_PFM), _NC_ },
    [kPOWER_Resource_CoreLvl]          = { _C(0U),       _C(0x11U),    _C(0x11U),    _C(0x11U),    _NC_ }, /* 0=use active-mode level */
    [kPOWER_Resource_Ldo0V8Mode]       = { _C(LDO_HP),   _C(LDO_HP),   _C(LDO_LP),   _C(LDO_LP),   _NC_ },
    [kPOWER_Resource_Ldo1V8Mode]       = { _C(LDO_HP),   _C(LDO_HP),   _C(LDO_LP),   _C(LDO_LP),   _NC_ },
    [kPOWER_Resource_LdoVdda1V8Mode]   = { _C(LDO_HP),   _C(LDO_HP),   _C(LDO_LP),   _C(LDO_LP),   _NC_ },
    [kPOWER_Resource_Hqref]            = { _C(true),     _C(true),     _C(true),     _C(true),     _NC_ }, /* HQREF ON in Sleep/DeepSleep; off only in PMU-RET (Power Down) */
    [kPOWER_Resource_Sensors]          = { _C(0U),       _C(0U),       _C(0U),       _C(0U),       _NC_ }, /* all sensors off */
    /* ---- Clock source gating (CSRCCFG_STBY + CSRCCFG_HSK_SEL) [EXPERIMENTAL] ---- */
    /* ON(c0,c1,c2)=retain enabled; OFF(c0,c1,c2)=cut off. Per-cell CMC routing:
     * (1,0,0)=CMC0 only (this example); e.g. ON(1,1,1) routes to all three CMCs. */
    /* DMA wakeup keeps its historical clock-source differences through runtime overrides. */
    /* PD column: _NC_ - clock-source standby state in Power Down is inlined by the driver. */
    /*                               [0]Slp          [1]DS1        [2]DS2        [3]DS3         [4]PD */
    [kPOWER_Resource_ClkLdoa0V8]  = { ON(1,0,0), ON(1,0,0),  OFF(1,0,0), OFF(1,0,0), _NC_ },
    [kPOWER_Resource_ClkFro192M]  = { ON(1,0,0), ON(1,0,0),  OFF(1,0,0), OFF(1,0,0), _NC_ },
    [kPOWER_Resource_ClkFro12M]   = { ON(1,0,0), ON(1,0,0),  OFF(1,0,0), OFF(1,0,0), _NC_ },
    [kPOWER_Resource_ClkMainPll]  = { ON(1,0,0), OFF(1,0,0), OFF(1,0,0), OFF(1,0,0), _NC_ }, /* DIV4 auto-restored on wakeup */
    [kPOWER_Resource_ClkCorePll]  = { ON(1,0,0), OFF(1,0,0), OFF(1,0,0), OFF(1,0,0), _NC_ },
    [kPOWER_Resource_ClkSysPll]   = { ON(1,0,0), OFF(1,0,0), OFF(1,0,0), OFF(1,0,0), _NC_ },
    [kPOWER_Resource_ClkLdoq0V8]  = { ON(1,0,0), OFF(1,0,0), OFF(1,0,0), OFF(1,0,0), _NC_ },
    [kPOWER_Resource_ClkSxosc]    = { ON(1,0,0), OFF(1,0,0), OFF(1,0,0), OFF(1,0,0), _NC_ },
    [kPOWER_Resource_ClkFro12MLp] = { ON(1,0,0), ON(1,0,0),  OFF(1,0,0), OFF(1,0,0), _NC_ },
    /* ---- Memory slice modes (MEMCON_SLICE.MEM_SLPCFG) [EXPERIMENTAL] ---- */
    /* Sleep does not configure MEMCON (memories stay on, no supply removed) -> Sleep column NC. */
    /* Deep Sleep: Retention, except a domain's memory is PowerDown once its power domain is
     * gated off (NPU/COMM/MEDIA TCM from DS2; M85 TCM in DS3) - an unpowered domain can't retain.
     * The other 7 SRAMCTL blocks (S2 B1-B7) retain in DS1 but power down from DS2 onward. */
    /* PD column: _NC_ for every non-retaining memory row - those blocks lose their supply in
     * PD regardless of MEM_SLPCFG. */
    /* WakeSram / ComputeSram01: driver reads config->setWakeSramMode / setComputeSram01Mode in
     * both POWER_EnterDeepSleep() and POWER_EnterPowerDown().
     *   WakeSram      - DS: Active (always-on WAKE_SS makes it safe), PD: Retention.
     *   ComputeSram01 - DS/PD: Retention (PowerDown risks losing live stack/heap data). */
    [kPOWER_Resource_WakeSram]         = { _NC_,        _C(MEM_ACT), _C(MEM_ACT), _C(MEM_ACT), _C(MEM_RET) },
    [kPOWER_Resource_ComputeSram01]    = { _NC_,        _C(MEM_RET), _C(MEM_RET), _C(MEM_RET), _C(MEM_RET) },
    [kPOWER_Resource_Ocram0M23]        = { _NC_,        _C(MEM_RET), _C(MEM_PD),  _C(MEM_PD),  _NC_ },
    [kPOWER_Resource_Ocram0M4]         = { _NC_,        _C(MEM_RET), _C(MEM_PD),  _C(MEM_PD),  _NC_ },
    [kPOWER_Resource_Ocram0M5]         = { _NC_,        _C(MEM_RET), _C(MEM_PD),  _C(MEM_PD),  _NC_ },
    [kPOWER_Resource_Ocram1M0]         = { _NC_,        _C(MEM_RET), _C(MEM_PD),  _C(MEM_PD),  _NC_ },
    [kPOWER_Resource_Ocram1M1]         = { _NC_,        _C(MEM_RET), _C(MEM_PD),  _C(MEM_PD),  _NC_ },
    [kPOWER_Resource_Ocram2M0]         = { _NC_,        _C(MEM_RET), _C(MEM_PD),  _C(MEM_PD),  _NC_ },
    [kPOWER_Resource_Ocram2M1]         = { _NC_,        _C(MEM_RET), _C(MEM_PD),  _C(MEM_PD),  _NC_ },
    [kPOWER_Resource_LlcCache]         = { _NC_,        _C(MEM_RET), _C(MEM_RET), _C(MEM_RET), _NC_ },
    [kPOWER_Resource_Audio8Kb]         = { _NC_,        _C(MEM_RET), _C(MEM_RET), _C(MEM_RET), _NC_ },
    [kPOWER_Resource_M85Tcm]           = { _NC_,        _C(MEM_RET), _C(MEM_RET), _C(MEM_PD),  _NC_ }, /* CPU off in DS3 -> PowerDown */
    [kPOWER_Resource_NpuTcm]           = { _NC_,        _C(MEM_RET), _C(MEM_PD),  _C(MEM_PD),  _NC_ }, /* NPU off from DS2 -> PowerDown */
    /* ---- Root clock standby enable (RCGCFG_STBY + RCGCFG_HSK_SEL) [EXPERIMENTAL] ---- */
    /* Active-high register: ON(c0,c1,c2)=bit=1 (enabled); OFF(c0,c1,c2)=bit=0 (gated).
     * Opposite polarity to CSRCCFG_STBY. Deep Sleep gates all root clocks except WAKE_1M/2M. */
    /* PD column: _NC_ - root-clock standby state in Power Down is inlined by the driver. */
    /*                               [0]Slp          [1]DS1        [2]DS2        [3]DS3         [4]PD */
    [kPOWER_Resource_RcgCompute]  = { ON(1,0,0), OFF(1,0,0), OFF(1,0,0), OFF(1,0,0), _NC_ },
    [kPOWER_Resource_RcgMain]     = { ON(1,0,0), OFF(1,0,0), OFF(1,0,0), OFF(1,0,0), _NC_ },
    [kPOWER_Resource_RcgWake]     = { ON(1,0,0), ON(1,0,0),  ON(1,0,0),  ON(1,0,0),  _NC_ }, /* WAKE_SS stays on */
    [kPOWER_Resource_RcgComm]     = { ON(1,0,0), OFF(1,0,0), OFF(1,0,0), OFF(1,0,0), _NC_ },
    [kPOWER_Resource_RcgMedia]    = { ON(1,0,0), OFF(1,0,0), OFF(1,0,0), OFF(1,0,0), _NC_ },
    [kPOWER_Resource_RcgAudio]    = { ON(1,0,0), OFF(1,0,0), OFF(1,0,0), OFF(1,0,0), _NC_ },
    [kPOWER_Resource_RcgWake1M]   = { ON(1,0,0), ON(1,0,0),  ON(1,0,0),  ON(1,0,0),  _NC_ }, /* always on: WAKE 1M timer */
    [kPOWER_Resource_RcgWake2M]   = { ON(1,0,0), ON(1,0,0),  ON(1,0,0),  ON(1,0,0),  _NC_ }, /* always on: WAKE 2M timer */
};

/*! @brief Render a non-raw cell value as its short display string. */
/*! Per-mode descriptors, indexed by APP_LP_* code (index 0 unused). */
const app_lp_mode_t APP_LP_MODES[APP_LP_MODE_COUNT] = {
    [APP_LP_SLEEP] = { "Sleep",             false, "WFI entered", "wake; run mode preserved",
                        kPOWER_Sleep,         kAPP_StandbyModeIdx_Sleep },
    [APP_LP_DS1]   = { "Deep Sleep 1",      true,  "WFI entered", "PMU restored",
                        kPOWER_DeepSleep,     kAPP_StandbyModeIdx_DS1 },
    [APP_LP_DS2]   = { "Deep Sleep 2",      true,  "WFI entered", "PMU restored",
                        kPOWER_DeepSleep,     kAPP_StandbyModeIdx_DS2 },
    [APP_LP_DS3]   = { "Deep Sleep 3",      true,  "WFI entered", "PMU restored",
                        kPOWER_DeepSleep,     kAPP_StandbyModeIdx_DS3 },
    [APP_LP_PD]    = { "Power Down",        true,  "WFI entered; system resets on wakeup", NULL,
                        kPOWER_PowerDown,     kAPP_StandbyModeIdx_Sleep /* unused: PD has no standby column */ },
    [APP_LP_DPD1]  = { "Deep Power Down 1", true,  "VBAT SRAM retained; VBAT-domain wakeup; no return", NULL,
                        kPOWER_DeepPowerDown, kAPP_StandbyModeIdx_Sleep /* unused */ },
    [APP_LP_DPD2]  = { "Deep Power Down 2", true,  "VBAT SRAM lost; VBAT-domain wakeup; no return",     NULL,
                        kPOWER_DeepPowerDown, kAPP_StandbyModeIdx_Sleep /* unused */ },
};

/*******************************************************************************
 * Table-driven config builders' field maps
 *
 * Each APP_ApplyFieldMap() call (power_mode_switch.c) iterates a
 * g_powerModeTable column, applying every valid cell to the matching struct
 * field via one of the maps below.  To change a mode's resource configuration,
 * edit the corresponding cell in g_powerModeTable above - no code change
 * required.
 ******************************************************************************/

/*
 * Data-driven Sleep / Deep Sleep field applicator.
 *
 * power_sleep_config_t and power_deep_sleep_config_t share the leading fields (domain
 * events, PMU analog, clock-source enables, root-clock enables) with the same names
 * and the same layout up to the point where Sleep ends. Deep Sleep then adds 8 memcon
 * `set*Mode` fields that Sleep does not have (Sleep never programs MEMCON).
 *
 * Every setter below is generated against `power_deep_sleep_config_t` (the wider
 * struct) and assigns through the real field name, so a typo'd field or a field
 * whose type later changes fails to compile instead of silently writing the wrong
 * byte width into memory (the risk with an offsetof + raw pointer write). This is
 * safe for Sleep too: `g_powerModeTable[memory_row][kAPP_StandbyModeIdx_Sleep]` is
 * `_NC_` (valid=false) for every memcon row, so a Sleep-sized buffer is never handed
 * to a memcon setter.
 *
 * Resources that have no field in these structs (PD/DPD-only rows) simply have no
 * entry below, so their g_slpFieldMap[] slot keeps its zero-init `{ NULL }` and
 * APP_ApplyFieldMap() skips them.
 *
 * X(resource, field, type): resource is the kPOWER_Resource_<resource> suffix,
 * field is the power_deep_sleep_config_t member name, type is that member's
 * declared type (used to cast the table's uint32_t cell value on assignment).
 */
#define APP_SLP_FIELD_LIST(X)                                                \
    X(CpuDomain,      setCpuDomainEvent,         pdcon_event_t)             \
    X(NpuDomain,      setNpuDomainEvent,         pdcon_event_t)             \
    X(CommDomain,     setCommDomainEvent,        pdcon_event_t)             \
    X(MediaDomain,    setMediaDomainEvent,       pdcon_event_t)             \
    X(PmuMode,        setPmuMode,                uint8_t)                  \
    X(DcdcMode,       setDcdcMode,               pmu_dcdc_mode_t)          \
    X(CoreLvl,        setCoreLevel,              uint8_t)                  \
    X(Ldo0V8Mode,     setLdo0V8Mode,             pmu_ldo_mode_t)           \
    X(Ldo1V8Mode,     setLdo1V8Mode,             pmu_ldo_mode_t)           \
    X(LdoVdda1V8Mode, setLdoVdda1V8Mode,         uint8_t)                  \
    X(Hqref,          enableHqref,               bool)                    \
    X(Sensors,        enableSensors,             uint8_t)                  \
    X(ClkLdoa0V8,     enableLdoa0V8ClockSource,  bool)                     \
    X(ClkFro192M,     enableFro192MClockSource,  bool)                     \
    X(ClkFro12M,      enableFro12MClockSource,   bool)                     \
    X(ClkMainPll,     enableMainPllClockSource,  bool)                     \
    X(ClkCorePll,     enableCorePllClockSource,  bool)                     \
    X(ClkSysPll,      enableSysPllClockSource,   bool)                     \
    X(ClkLdoq0V8,     enableLdoq0V8ClockSource,  bool)                     \
    X(ClkSxosc,       enableSxoscClockSource,    bool)                     \
    X(ClkFro12MLp,    enableFro12MLpClockSource, bool)                     \
    X(WakeSram,       setWakeSramMode,           memcon_power_mode_t)     \
    X(ComputeSram01,  setComputeSram01Mode,      memcon_power_mode_t)     \
    X(Ocram0M23,      setOcram0M23Mode,          memcon_power_mode_t)     \
    X(Ocram0M4,       setOcram0M4Mode,           memcon_power_mode_t)     \
    X(Ocram0M5,       setOcram0M5Mode,           memcon_power_mode_t)     \
    X(Ocram1M0,       setOcram1M0Mode,           memcon_power_mode_t)     \
    X(Ocram1M1,       setOcram1M1Mode,           memcon_power_mode_t)     \
    X(Ocram2M0,       setOcram2M0Mode,           memcon_power_mode_t)     \
    X(Ocram2M1,       setOcram2M1Mode,           memcon_power_mode_t)     \
    X(LlcCache,       setLlcCacheMode,           memcon_power_mode_t)     \
    X(Audio8Kb,       setAudio8KbMode,           memcon_power_mode_t)     \
    X(M85Tcm,         setM85TcmMode,             memcon_power_mode_t)     \
    X(NpuTcm,         setNpuTcmMode,             memcon_power_mode_t)     \
    X(RcgCompute,     enableComputeRootClock,    bool)                    \
    X(RcgMain,        enableMainRootClock,       bool)                    \
    X(RcgWake,        enableWakeRootClock,       bool)                    \
    X(RcgComm,        enableCommRootClock,       bool)                    \
    X(RcgMedia,       enableMediaRootClock,      bool)                    \
    X(RcgAudio,       enableAudioRootClock,      bool)                    \
    X(RcgWake1M,      enableWake1MRootClock,     bool)                    \
    X(RcgWake2M,      enableWake2MRootClock,     bool)

#define APP_DEFINE_SLP_SETTER(res, field, type)                  \
    static void APP_SetSlp_##res(void *cfg, uint32_t value)      \
    {                                                             \
        ((power_deep_sleep_config_t *)cfg)->field = (type)value; \
    }
APP_SLP_FIELD_LIST(APP_DEFINE_SLP_SETTER)

#define APP_SLP_FIELD_ENTRY(res, field, type) [kPOWER_Resource_##res] = { APP_SetSlp_##res },
const app_field_map_entry_t g_slpFieldMap[kPOWER_Resource_COUNT] = {
    APP_SLP_FIELD_LIST(APP_SLP_FIELD_ENTRY)
};

/*!
 * @brief PD field map - setters into power_down_config_t for the two Power-Down-
 * configurable resources. Every other resource has no entry (NULL setter, via the
 * array's zero-init default), so APP_ApplyFieldMap() silently skips their PD-column
 * cells.
 *
 * Kept separate from g_slpFieldMap because power_down_config_t and
 * power_deep_sleep_config_t are different struct types - a shared map would need a
 * setter per struct anyway, which is exactly what this second small map is.
 */
#define APP_PD_FIELD_LIST(X)                                     \
    X(WakeSram,      setWakeSramMode,      memcon_power_mode_t) \
    X(ComputeSram01, setComputeSram01Mode, memcon_power_mode_t)

#define APP_DEFINE_PD_SETTER(res, field, type)               \
    static void APP_SetPd_##res(void *cfg, uint32_t value)   \
    {                                                        \
        ((power_down_config_t *)cfg)->field = (type)value;   \
    }
APP_PD_FIELD_LIST(APP_DEFINE_PD_SETTER)

#define APP_PD_FIELD_ENTRY(res, field, type) [kPOWER_Resource_##res] = { APP_SetPd_##res },
const app_field_map_entry_t g_pdFieldMap[kPOWER_Resource_COUNT] = {
    APP_PD_FIELD_LIST(APP_PD_FIELD_ENTRY)
};

const app_hsk_field_t g_hskFieldMap[kPOWER_Resource_COUNT] = {
    /* CSRCCFG_HSK_SEL (regIdx=1) */
    [kPOWER_Resource_ClkLdoa0V8]  = { 1U,  0U },
    [kPOWER_Resource_ClkFro192M]  = { 1U,  4U },
    [kPOWER_Resource_ClkFro12M]   = { 1U,  8U },
    [kPOWER_Resource_ClkMainPll]  = { 1U, 12U },
    [kPOWER_Resource_ClkCorePll]  = { 1U, 16U },
    [kPOWER_Resource_ClkSysPll]   = { 1U, 20U },
    [kPOWER_Resource_ClkLdoq0V8]  = { 1U, 24U },
    [kPOWER_Resource_ClkSxosc]    = { 1U, 28U },
    /* CSRCCFG_HSK_SEL1 (regIdx=2) */
    [kPOWER_Resource_ClkFro12MLp] = { 2U,  0U },
    /* RCGCFG_HSK_SEL (regIdx=0) */
    [kPOWER_Resource_RcgCompute]  = { 0U,  0U },
    [kPOWER_Resource_RcgMain]     = { 0U,  4U },
    [kPOWER_Resource_RcgWake]     = { 0U,  8U },
    [kPOWER_Resource_RcgComm]     = { 0U, 12U },
    [kPOWER_Resource_RcgMedia]    = { 0U, 16U },
    [kPOWER_Resource_RcgAudio]    = { 0U, 20U },
    [kPOWER_Resource_RcgWake1M]   = { 0U, 24U },
    [kPOWER_Resource_RcgWake2M]   = { 0U, 28U },
};

/*!
 * @brief SoC handshake routing - HSK_SEL routing and PDCON per-domain handshake mask.
 *
 * rcgcfgHskSel / csrccfgHskSel / csrccfgHskSel1 = 0x11111111 / 0x11111111 /
 * 0x1 routes every root clock and clock source to CMC0 only (this example).
 * Use 0x77777777 / 0x77777777 / 0x7 to route to all three CMCs (CMC0 CPU,
 * CMC1 MAIN, CMC2 WAKE) - the RT2660 reference handshake routing.
 *
 * domainHsk[] = kPDCON_HandshakeAll for all 6 domains means every PDCON
 * domain transition handshakes with all units.
 *
 * Edit any field below for a non-reference board.
 */
power_handshake_routing_config_t g_handshakeRoutingCfg = {
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

/*! @} */
