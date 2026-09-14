/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_power.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_POWER_NAME                                                             \
    {                                                                              \
        "WFI", "Sleep", "Deep Sleep", "Power Down", "Deep Power Down"              \
    }
#define APP_TARGET_POWER_NUM (5U)

/*
 * VBAT domain LPTMR: the demo's single wakeup timer across every low power mode
 * (WFI, Sleep, Deep Sleep, Power Down, Deep Power Down). It lives in the always-on
 * VBAT domain and is clocked from ULP32K (FRO32K/OSC32K, ~32768 Hz), so it keeps
 * counting even in Deep Power Down where VDD_CORE and the WAKE domain are off.
 * Wired as the power_manager framework's low-power timer (PM_RegisterTimerController),
 * not a generic PM_InitWakeupSource wakeup source -- see APP_LptmrStart/APP_LptmrStop.
 * Reuses the same VBAT LPTMR already proven by power_mode_switch_rt2660's app.h.
 */
#define APP_VBAT_LPTMR_BASE        VBAT__LPTMR
#define APP_VBAT_LPTMR_IRQ         VBAT_LPTMR_IRQn
#define APP_VBAT_LPTMR_IRQ_HANDLER VBAT_LPTMR_IRQHandler
#define APP_LPTMR_CLK_HZ           32768U /* ULP32K count rate */
#define APP_LPTMR_TIMEOUT_MS       5000U  /* 5 second wakeup timeout */

/*
 * Per-mode resource constraint lists.
 *
 * The FIRST argument to PM_SetConstraints/PM_ReleaseConstraints is the power mode INDEX
 * (ceiling): PM_findDeepestState() picks the deepest state <= ceiling that passes the
 * fixConstraintsMask check.  Setting the ceiling to the intended mode guarantees the exact
 * mode is entered.
 *
 * Each list exhaustively specifies the DESIRED state of every configurable resource in
 * that mode.  Constraints with an _ON / _ACTIVE / _RETENTION level are tracked by the
 * framework and passed to PM_DEV_EnterLowPowerMode() as pSoftRescMask so the device port
 * programs the hardware accordingly.  Constraints with a _OFF / _POWER_DOWN / _POWER_GATE
 * level use PM_RESOURCE_OFF, which the framework accepts (iterates) but does not track --
 * they serve as self-documenting statements of expected hardware state and are no-ops from
 * a constraint-tracking perspective; the device port uses its per-state baseline defaults
 * for those resources.
 */

/*! WFI: ceiling at PM_LP_STATE_WFI=0.  CM85 must be idle (not executing). */
#define APP_WFI_CONSTRAINTS  1U, PM_RESC_CM85_IDLE

/*! Sleep: ceiling at PM_LP_STATE_SLEEP=1.  CM85 and NPU clock-gated (domain on). */
#define APP_SLEEP_CONSTRAINTS \
    2U, PM_RESC_CM85_CLOCK_GATE, PM_RESC_NPU_CLOCK_GATE

/*! Deep Sleep: ceiling at PM_LP_STATE_DEEP_SLEEP=2.  Full resource specification:
 *  CM85/NPU clock-gated; Wake/Main/Media/Comm domains on; WAKE SRAM active;
 *  all other SRAM blocks in retention; Compute/Main/Comm/Media/Audio root clocks
 *  gated; Wake/Wake1M/Wake2M on; LDOA_0V8/FRO192M/FRO12M/FRO12MLP clock sources on;
 *  PLLs and other high-power clock sources off. */
#define APP_DEEP_SLEEP_CONSTRAINTS \
    36U, \
    PM_RESC_CM85_CLOCK_GATE, PM_RESC_NPU_CLOCK_GATE, \
    PM_RESC_WAKE_SRAM_ACTIVE, \
    PM_RESC_WAKE_DOMAIN_ON, PM_RESC_MAIN_DOMAIN_ON, PM_RESC_MEDIA_DOMAIN_ON, PM_RESC_COMM_DOMAIN_ON, \
    PM_RESC_OCRAM0_M23_RETENTION, PM_RESC_OCRAM0_M4_RETENTION, PM_RESC_OCRAM0_M5_RETENTION, \
    PM_RESC_OCRAM1_M0_RETENTION, PM_RESC_OCRAM1_M1_RETENTION, \
    PM_RESC_OCRAM2_M0_RETENTION, PM_RESC_OCRAM2_M1_RETENTION, \
    PM_RESC_LLC_CACHE_RETENTION, PM_RESC_AUDIO_8KB_RETENTION, \
    PM_RESC_M85_TCM_RETENTION, PM_RESC_COMM_TCM_RETENTION, PM_RESC_MEDIA_TCM_RETENTION, \
    PM_RESC_RCG_COMPUTE_OFF, PM_RESC_RCG_MAIN_OFF, PM_RESC_RCG_WAKE_ON, \
    PM_RESC_RCG_COMM_OFF, PM_RESC_RCG_MEDIA_OFF, PM_RESC_RCG_AUDIO_OFF, \
    PM_RESC_RCG_WAKE_1M_ON, PM_RESC_RCG_WAKE_2M_ON, \
    PM_RESC_CLK_LDOA_0V8_ON, PM_RESC_CLK_FRO_192M_ON, PM_RESC_CLK_FRO_12M_ON, \
    PM_RESC_CLK_MAIN_PLL_OFF, PM_RESC_CLK_CORE_PLL_OFF, PM_RESC_CLK_SYSPLL_OFF, \
    PM_RESC_CLK_LDOQ_0V8_OFF, PM_RESC_CLK_SXOSC_OFF, PM_RESC_CLK_FRO_12M_LP_ON

/*! Power Down: ceiling at PM_LP_STATE_POWER_DOWN=3.  Full resource specification:
 *  CM85/NPU power-gated (domain off); Wake domain on; Main/Media/Comm domains off;
 *  WakeSram and ComputeSram01 retained; all other SRAM lost; Compute/Comm/Media/Audio
 *  root clocks off; Wake/Wake1M/Wake2M/Main root clocks on (hardcoded by driver);
 *  all 9 clock sources on (driver uses POWER_CSRCCFG_STBY_ALL_ON in PD). */
#define APP_POWER_DOWN_CONSTRAINTS \
    38U, \
    PM_RESC_CM85_POWER_GATE, PM_RESC_NPU_POWER_GATE, \
    PM_RESC_WAKE_DOMAIN_ON, PM_RESC_MAIN_DOMAIN_OFF, PM_RESC_MEDIA_DOMAIN_OFF, PM_RESC_COMM_DOMAIN_OFF, \
    PM_RESC_WAKE_SRAM_RETENTION, PM_RESC_COMPUTE_SRAM01_RETENTION, \
    PM_RESC_OCRAM0_M23_POWER_DOWN, PM_RESC_OCRAM0_M4_POWER_DOWN, PM_RESC_OCRAM0_M5_POWER_DOWN, \
    PM_RESC_OCRAM1_M0_POWER_DOWN, PM_RESC_OCRAM1_M1_POWER_DOWN, \
    PM_RESC_OCRAM2_M0_POWER_DOWN, PM_RESC_OCRAM2_M1_POWER_DOWN, \
    PM_RESC_LLC_CACHE_POWER_DOWN, PM_RESC_AUDIO_8KB_POWER_DOWN, \
    PM_RESC_M85_TCM_POWER_DOWN, PM_RESC_NPU_TCM_POWER_DOWN, \
    PM_RESC_COMM_TCM_POWER_DOWN, PM_RESC_MEDIA_TCM_POWER_DOWN, \
    PM_RESC_RCG_MAIN_ON, PM_RESC_RCG_WAKE_ON, PM_RESC_RCG_WAKE_1M_ON, PM_RESC_RCG_WAKE_2M_ON, \
    PM_RESC_RCG_COMPUTE_OFF, PM_RESC_RCG_COMM_OFF, PM_RESC_RCG_MEDIA_OFF, PM_RESC_RCG_AUDIO_OFF, \
    PM_RESC_CLK_LDOA_0V8_ON, PM_RESC_CLK_FRO_192M_ON, PM_RESC_CLK_FRO_12M_ON, \
    PM_RESC_CLK_MAIN_PLL_ON, PM_RESC_CLK_CORE_PLL_ON, PM_RESC_CLK_SYSPLL_ON, \
    PM_RESC_CLK_LDOQ_0V8_ON, PM_RESC_CLK_SXOSC_ON, PM_RESC_CLK_FRO_12M_LP_ON

/*! Deep Power Down: ceiling at PM_LP_STATE_DEEP_POWER_DOWN=4.  Everything off except
 *  VbatSram (retain = DPD1; power down = DPD2).  All domains, SRAM, root clocks and
 *  clock sources are powered off / gated -- stated explicitly for documentation. */
#define APP_DEEP_POWER_DOWN_CONSTRAINTS \
    39U, \
    PM_RESC_CM85_POWER_GATE, PM_RESC_NPU_POWER_GATE, \
    PM_RESC_WAKE_DOMAIN_OFF, PM_RESC_MAIN_DOMAIN_OFF, PM_RESC_MEDIA_DOMAIN_OFF, PM_RESC_COMM_DOMAIN_OFF, \
    PM_RESC_WAKE_SRAM_POWER_DOWN, PM_RESC_COMPUTE_SRAM01_POWER_DOWN, \
    PM_RESC_OCRAM0_M23_POWER_DOWN, PM_RESC_OCRAM0_M4_POWER_DOWN, PM_RESC_OCRAM0_M5_POWER_DOWN, \
    PM_RESC_OCRAM1_M0_POWER_DOWN, PM_RESC_OCRAM1_M1_POWER_DOWN, \
    PM_RESC_OCRAM2_M0_POWER_DOWN, PM_RESC_OCRAM2_M1_POWER_DOWN, \
    PM_RESC_LLC_CACHE_POWER_DOWN, PM_RESC_AUDIO_8KB_POWER_DOWN, \
    PM_RESC_M85_TCM_POWER_DOWN, PM_RESC_NPU_TCM_POWER_DOWN, \
    PM_RESC_COMM_TCM_POWER_DOWN, PM_RESC_MEDIA_TCM_POWER_DOWN, \
    PM_RESC_VBAT_SRAM_RETAIN, \
    PM_RESC_RCG_COMPUTE_OFF, PM_RESC_RCG_MAIN_OFF, PM_RESC_RCG_WAKE_OFF, \
    PM_RESC_RCG_COMM_OFF, PM_RESC_RCG_MEDIA_OFF, PM_RESC_RCG_AUDIO_OFF, \
    PM_RESC_RCG_WAKE_1M_OFF, PM_RESC_RCG_WAKE_2M_OFF, \
    PM_RESC_CLK_LDOA_0V8_OFF, PM_RESC_CLK_FRO_192M_OFF, PM_RESC_CLK_FRO_12M_OFF, \
    PM_RESC_CLK_MAIN_PLL_OFF, PM_RESC_CLK_CORE_PLL_OFF, PM_RESC_CLK_SYSPLL_OFF, \
    PM_RESC_CLK_LDOQ_0V8_OFF, PM_RESC_CLK_SXOSC_OFF, PM_RESC_CLK_FRO_12M_LP_OFF
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t APP_ControlCallback_notify(pm_event_type_t eventType, uint8_t powerState, void *data);
void APP_InitWakeupSource(void);
void APP_RegisterNotify(void);
uint32_t APP_GetWakeupTimeout(void);
void APP_SetConstraints(uint8_t powerMode);
void APP_ReleaseConstraints(uint8_t powerMode);
void APP_LptmrStart(uint64_t timeoutUs);
void APP_LptmrStop(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
