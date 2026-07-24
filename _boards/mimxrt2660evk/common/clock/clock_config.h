/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief Default board clock boot entry. Forwards to BOARD_BootClockHPRUN().
 *
 * SDK examples' main() calls this. Programs the SoC to the Over Drive Run FBB
 * (HpRun) operating point so existing examples see no behavior change.
 */
void BOARD_InitBootClocks(void);

/*!
 * @brief Over Drive Run FBB (HpRun) clock configuration.
 *
 * Programs the SoC clocks to the first column ("OD Run FBB") of
 * RT2660_Power_Mode_Spec_V0.9:
 *   CM85 = 1000 MHz  (MAINPLL_DIVOUT1 -> PLL_PFDX)
 *   NPU  = 792 MHz   (COREPLL_OUT; spec-nominal 800 MHz, ~1% low)
 *   MAIN / CMPT = 333 MHz  (MAINPLL_DIVOUT1 -> PLLPFDX / 3)
 *   MEDIA = 333 MHz  (MAINPLL_DIVOUT1 -> PLLPFDX / 3)
 *   COMM  = 200 MHz  (MAINPLL_DIV5 -> MAINPLLDIVX / 2)
 *   AUDIO = 200 MHz  (MAINPLL_DIV5 -> MAINPLLDIVX / 2)
 *   WAKE  = 40 MHz   (MAINPLL_DIV5 / 10)
 *
 * Precondition (caller responsibility, not enforced): DCDC = 0.9 V,
 * FBB enabled, PMU MODE = HP. Callers switching between HP/NP/LP modes
 * must reset the SoC first (this file does not support runtime mode-switching).
 */
void BOARD_BootClockHPRUN(void);

/*!
 * @brief Normal Drive Run FBB (NpRun) clock configuration.
 *
 * Programs the SoC clocks to the second column ("ND Run FBB") of
 * RT2660_Power_Mode_Spec_V0.9:
 *   CM85 = 792 MHz   (COREPLL_OUT; spec-nominal 800 MHz, ~1% low)
 *   NPU  = 666 MHz   (MAINPLL_DIVOUT1)
 *   MAIN / CMPT = 264 MHz  (COREPLL_OUT / 3; spec-nominal 266 MHz)
 *   MEDIA = 250 MHz  (SYSPLL_DIVOUT2 / 2)
 *   COMM  = 200 MHz  (MAINPLL_DIV5 -> MAINPLLDIVX / 2)
 *   AUDIO = 200 MHz  (MAINPLL_DIV5 -> MAINPLLDIVX / 2)
 *   WAKE  = 40 MHz   (MAINPLL_DIV5 / 10)
 *
 * Precondition (caller responsibility, not enforced): DCDC = 0.8 V,
 * FBB enabled, PMU MODE = HP.
 */
void BOARD_BootClockNPRUN(void);

/*!
 * @brief Normal Drive Run ZBB (LpRun) clock configuration.
 *
 * Programs the SoC clocks to the third column ("ND Run ZBB") of
 * RT2660_Power_Mode_Spec_V0.9:
 *   CM85 = 600 MHz   (COREPLL_OUT)
 *   NPU  = 400 MHz   (MAINPLL_DIVOUT1)
 *   MAIN / CMPT = 200 MHz  (COREPLL_OUT / 3)
 *   MEDIA = 250 MHz  (SYSPLL_DIVOUT2 / 2)
 *   COMM  = 200 MHz  (MAINPLL_DIV5 -> MAINPLLDIVX / 2)
 *   AUDIO = 200 MHz  (MAINPLL_DIV5 -> MAINPLLDIVX / 2)
 *   WAKE  = 40 MHz   (MAINPLL_DIV5 / 10)
 *
 * Precondition (caller responsibility, not enforced): DCDC = 0.8 V,
 * FBB DISABLED (ZBB), PMU MODE = HP. LPRUN reprograms CorePLL to 600 MHz
 * (vs. 792 MHz for HPRUN/NPRUN).
 */
void BOARD_BootClockLPRUN(void);

/*!
 * @brief Deprecated alias for BOARD_BootClockHPRUN.
 *
 * Historically implemented the OD FBB (HpRun) column. New code should call
 * BOARD_BootClockHPRUN() directly.
 */
void BOARD_BootClockRUN(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */
