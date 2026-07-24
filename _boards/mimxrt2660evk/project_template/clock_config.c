/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "clock_config.h"
#include "fsl_clock.h"
#include "fsl_iomuxc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* BOARD_BootClockRUN target frequencies
 *
 *  SXOSC crystal      :  24 MHz   (reference for all PLLs)
 *  FRO 192 MHz        : 192 MHz   (FRO192M / FRO96M / FRO48M / FRO24M roots)
 *  FRO  12 MHz        :  12 MHz   (backup low-power clock)
 *  Core PLL           : 960 MHz   (24 MHz × 40 → CPU_ROOTCLK, NPU_ROOTCLK)
 *  Main PLL VCO       :   2 GHz
 *    DIV4 (always)    : 500 MHz
 *    DIV8             : 250 MHz
 *    DIV10            : 200 MHz
 *    DIV20            : 100 MHz
 *    FRACOUT0 sel=20  : 400 MHz   (2000 × 4 / 20; PERI clocks)
 *    FRACOUT1 sel=10  : 800 MHz   (2000 × 4 / 10; high-speed PERI)
 *  Sys PLL VCO        :   2 GHz
 *    DIV8             : 250 MHz
 *    DIV10            : 200 MHz
 *    DIV20            : 100 MHz
 *    FRACOUT0 sel=20  : 400 MHz   (2000 × 4 / 20)
 *    FRACOUT1 sel=12  : 667 MHz   (2000 × 4 / 12; COMMBUS / AUDIOBUS)
 *
 * NOTE: CGUDIG root-clock mux/divider configuration (routing the above sources
 * to CGU root clocks such as CPU_ROOTCLK, MAINDIVX_ROOTCLK, PERI_ROOTCLKx, …)
 * must be performed separately once the CGUDIG driver is available.
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* SXOSC: 24 MHz crystal.
 * gmSel and cap-trim values are board-layout dependent — tune per schematic. */
static const clock_cguana_sxosc_config_t s_sxoscConfig = {
    .modeSel      = 0U,   /* crystal (differential) mode */
    .gmSel        = 4U,   /* moderate GM; adjust for crystal ESR */
    .xtal1CapTrim = 16U,  /* XTAL1 load cap trim [0..63] */
    .xtal2CapTrim = 16U,  /* XTAL2 load cap trim [0..63] */
    .detTrim      = 0U,
    .clkDiv2En    = false, /* 24 MHz output (no divide-by-2) */
};

/* FRO 192 MHz: OTWB=3 targets the 192 MHz band. */
static const clock_cguana_fro192m_config_t s_fro192mConfig = {
    .otwb = 3U,
};

/* FRO 12 MHz: OTWB=2 targets the 12 MHz band. */
static const clock_cguana_fro12m_config_t s_fro12mConfig = {
    .otwb = 2U,
};

/* Core PLL: F_OUT = 24 MHz × 40 = 960 MHz, HF VCO (858–1200 MHz range). */
static const clock_cguana_core_pll_config_t s_corePllConfig = {
    .startMode   = kCLOCK_CguanaPllStartFull,
    .vcoSelHf    = true,
    .refFreq     = kCLOCK_CguanaRefFreq24M,
    .loopDivNint = 40U,
    .postDivBy2  = false,
};

/* Main PLL: 2 GHz VCO, integer and fractional outputs enabled. */
static const clock_cguana_frac_pll_config_t s_mainPllConfig = {
    .startMode = kCLOCK_CguanaPllStartFull,
    .refFreq   = kCLOCK_CguanaRefFreq24M,
    .lowFreq   = 0U,    /* VCO = 2000 MHz */
    .div5En    = false,
    .div8En    = true,  /* 250 MHz */
    .div10En   = true,  /* 200 MHz */
    .div20En   = true,  /* 100 MHz */
    .fracDiv = {
        { .en = true,  .range = false, .sel = 20U }, /* 2000×4/20 = 400 MHz */
        { .en = true,  .range = false, .sel = 10U }, /* 2000×4/10 = 800 MHz */
        { .en = false, .range = false, .sel = 0U  }, /* disabled             */
    },
    .sscgEn = false,
    .sscg   = NULL,
};

/* Sys PLL: 2 GHz VCO, subset of outputs enabled. */
static const clock_cguana_frac_pll_config_t s_sysPllConfig = {
    .startMode = kCLOCK_CguanaPllStartFull,
    .refFreq   = kCLOCK_CguanaRefFreq24M,
    .lowFreq   = 0U,    /* VCO = 2000 MHz */
    .div5En    = false,
    .div8En    = true,  /* 250 MHz */
    .div10En   = true,  /* 200 MHz */
    .div20En   = true,  /* 100 MHz */
    .fracDiv = {
        { .en = true,  .range = false, .sel = 20U }, /* 2000×4/20 = 400 MHz         */
        { .en = true,  .range = false, .sel = 12U }, /* 2000×4/12 ≈ 667 MHz         */
        { .en = false, .range = false, .sel = 0U  }, /* disabled                    */
    },
    .sscgEn = false,
    .sscg   = NULL,
};

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
    BOARD_BootClockRUN();
}

/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/
void BOARD_BootClockRUN(void)
{
    /* Step 1: Enable FRO 12 MHz as early safe clock source. */
    CLOCK_InitFro12M(&s_fro12mConfig);

    /* Step 2: Enable FRO 192 MHz; provides FRO-based root clocks immediately. */
    CLOCK_InitFro192M(&s_fro192mConfig);

    /* Step 3: Enable SXOSC 24 MHz crystal; used as PLL reference. */
    CLOCK_InitSxosc(&s_sxoscConfig);

    /* Step 4: Core PLL → 960 MHz (CPU / NPU roots). */
    CLOCK_InitCorePll(&s_corePllConfig);

    /* Step 5: Main PLL → 2 GHz VCO, multiple fixed and fractional outputs. */
    CLOCK_InitMainPll(&s_mainPllConfig);

    /* Step 6: Sys PLL → 2 GHz VCO, secondary fixed and fractional outputs. */
    CLOCK_InitSysPll(&s_sysPllConfig);

    /* TODO: Configure CGUDIG root-clock mux and dividers to route the above
     * PLL outputs to subsystem roots (CPU_ROOTCLK, MAINDIVX_ROOTCLK,
     * PERI_ROOTCLKx, AUDIOBUS_ROOTCLK, …) once the CGUDIG driver is
     * implemented. */
}
