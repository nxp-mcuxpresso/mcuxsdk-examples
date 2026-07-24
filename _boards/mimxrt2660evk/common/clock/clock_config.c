/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "clock_config.h"
#include "fsl_clock.h"
#include "fsl_iomuxc.h"
#include "fsl_power.h"
#include "fsl_modcon.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* BOARD_BootClockRUN target frequencies (RUN 1000/333 mode).
 *
 *  SXOSC crystal      :  24 MHz   (reference for all PLLs)
 *  FRO 192 MHz        : 192 MHz   (FRO192M / FRO96M / FRO48M / FRO24M roots)
 *  FRO  12 MHz        :  12 MHz   (backup low-power clock)
 *  Core PLL           : 792 MHz   (24 x 33; LF VCO band 600-858 MHz)
 *  Main PLL VCO       :   2 GHz
 *    DIV4 (always)    : 500 MHz
 *    DIV5             : 400 MHz   (-> MAINPLL_DIVX / MAINPLLDIVX_ROOTCLK route)
 *    DIV8             : 250 MHz
 *    DIV10            : 200 MHz
 *    DIV20            : 100 MHz
 *    DIVOUT0 sel=12   : 666.67 MHz (2000 / 3.0; PERI_ROOTCLK0/4/5 source)
 *    DIVOUT1 sel=8    : 1000 MHz  (2000 / 2.0; PLL_PFDX -> MAIN_ROOTCLK)
 *    DIVOUT2 sel=10   : 800 MHz   (2000 / 2.5; PERI_ROOTCLK2/3/6)
 *  Sys PLL VCO        :   2 GHz
 *    DIV4 (always)    : 500 MHz
 *    DIV5             : 400 MHz   (-> SYSPLL_DIVX / SYSPLLDIVX_ROOTCLK route)
 *    DIV8             : 250 MHz   (ETH0/1_TRXCLK source via MAINPLLDIV8)
 *    DIV10            : 200 MHz   (SYSCON_PDMAIN_CLK)
 *    DIV20            : 100 MHz   (ETH_ROOTCLK)
 *    DIVOUT0 sel=8    : 1000 MHz  (2000 / 2.0)
 *    DIVOUT1 sel=8    : 1000 MHz  (2000 / 2.0)
 *    DIVOUT2 sel=16   : 500 MHz   (2000 / 4.0)
 *  Audio PLL          :  49.152 MHz (F_AVPLL = 786.432 MHz / POSTDIV=16; CCO band 6)
 *  Video PLL          :  70.640 MHz (F_AVPLL = 777.04  MHz / POSTDIV=11; CCO band 5, DNUM=0x181B4E82)
 *
 *  Selected CGUDIG outputs (programmed by ConfigCGUDig helpers):
 *    MAIN_ROOTCLK      = PLL_PFDX(MAINPLL_DIVOUT1) / 1 / 1 = 1000 MHz   (CGU root 30; div, sndDiv)
 *    MEDIABUS_ROOTCLK  = PLL_PFDX(MAINPLL_DIVOUT1) / 3 = 333.33 MHz
 *    NPU_ROOTCLK       = COREPLL_OUT                   =  792 MHz
 *    AUDIOBUS/COMMBUS  = MAINPLL_DIVX  / 2             =  200 MHz
 *    WAKEBUS_ROOTCLK   = MAINPLL_DIVX  / 10            =   40 MHz
 *    SYSCON_PDMAIN_CLK = SYSPLL_DIV10                  =  200 MHz
 *    ETH_ROOTCLK       = SYSPLL_DIV20                  =  100 MHz
 *
 * The analog sources (FROs, SXOSC, Core/Main/Sys/Audio/Video PLLs) are programmed
 * by ConfigCGUAna(). The CGUDig clock-root mux/dividers are programmed by
 * ConfigCGUDig() through seven domain helpers (CGU/CMPT/MAIN/MEDIA/AUDIO/COMM/WAKE),
 * one CLOCK_SetRootClock call per silicon CCM slice, in CCM slice index order.
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* TBD (board / HW team): SXOSC analog trim values below are placeholder defaults
 * that need to be confirmed against the mimxrt2660evk crystal and load-cap network:
 *   - gmSel: depends on crystal ESR; RM table 804 lists typical values per XTAL
 *     frequency (24 MHz -> 3 is the RM-recommended value, we currently use 4).
 *   - xtal1CapTrim / xtal2CapTrim: must match the on-board crystal load capacitance
 *     and PCB stray capacitance. Range is [0..63]; both pins should be set to the
 *     same value unless the schematic specifies asymmetric load caps.
 *   - detTrim: amplitude-detector trim; leave at 0 unless the HW team specifies
 *     a different setting for startup margin.
 * If the HW team has measured oscillation margin / start-up time on this board,
 * the calibrated values should replace the placeholders below. */
static const clock_cguana_sxosc_config_t s_sxoscConfig = {
    .modeSel      = kCLOCK_CguanaSxoscModeCrystal,
    .gmSel        = 4U,
    .xtal1CapTrim = 16U,
    .xtal2CapTrim = 16U,
    .detTrim      = 0U,
    .clkDiv2En    = false, /* 24 MHz output (no divide-by-2). */
};

/* FRO 192 MHz: OTWB=3 targets the 192 MHz band. */
static const clock_cguana_fro192m_config_t s_fro192mConfig = {
    .otwb = kCLOCK_CguanaFro192mOtwb192M,
};

/* FRO 12 MHz: OTWB=2 targets the 12 MHz band. */
static const clock_cguana_fro12m_config_t s_fro12mConfig = {
    .otwb = kCLOCK_CguanaFro12mOtwb12M,
};

/* Core PLL: F_OUT = 24 MHz x 33 = 792 MHz, LF VCO (600-858 MHz range).
 * Used by HPRUN and NPRUN. HPRUN NPU_ROOTCLK = COREPLL_OUT = 792 MHz (spec-nominal
 * 800 MHz per RT2660_Power_Mode_Spec_V0.9; ~1% low, engineer-approved tolerance).
 * NPRUN CM85 = COREPLL_OUT = 792 MHz (spec-nominal 800 MHz, same tolerance). */
static const clock_cguana_core_pll_config_t s_corePllConfig = {
    .startMode   = kCLOCK_CguanaPllStartFull,
    .vcoSelHf    = false,
    .refFreq     = kCLOCK_CguanaRefFreq24M,
    .loopDivNint = 33U,
    .postDivBy2  = false,
};

/* Core PLL (LPRUN variant): F_OUT = 24 MHz x 25 = 600 MHz, LF VCO low edge
 * (600 MHz is at the bottom of the 600-858 MHz LF band). Used only by LPRUN.
 * LPRUN CM85 = COREPLL_OUT = 600 MHz per RT2660_Power_Mode_Spec_V0.9 ND-ZBB column. */
static const clock_cguana_core_pll_config_t s_corePllConfig_lp = {
    .startMode   = kCLOCK_CguanaPllStartFull,
    .vcoSelHf    = false,
    .refFreq     = kCLOCK_CguanaRefFreq24M,
    .loopDivNint = 25U,
    .postDivBy2  = false,
};

/* Main PLL (HPRUN variant): 2 GHz VCO, integer and fractional outputs enabled.
 * DIVOUT1 = 1000 MHz -- required by HPRUN to feed PLL_PFDX -> MAIN_ROOTCLK = 1000 (CM85). */
static const clock_cguana_frac_pll_config_t s_mainPllConfig = {
    .startMode = kCLOCK_CguanaPllStartFull,
    .refFreq   = kCLOCK_CguanaRefFreq24M,
    .lowFreq   = kCLOCK_CguanaFracPllVco2000M,
    .div5En    = true, /* 400 MHz -- feeds MAINPLL_DIV5 -> MAINDIVX/MAINPLLDIVX route */
    .div8En    = true, /* 250 MHz */
    .div10En   = true, /* 200 MHz */
    .div20En   = true, /* 100 MHz */
    .fracDiv = {
            {.en = true, .range = false, .sel = 12U}, /* DIVOUT0: 2000/3.0  = 666.67 MHz */
            {.en = true, .range = false, .sel = 8U},  /* DIVOUT1: 2000/2.0  = 1000 MHz   */
            {.en = true, .range = false, .sel = 10U}, /* DIVOUT2: 2000/2.5  = 800 MHz    */
    },
    .sscgEn = false,
    .sscg   = NULL,
};

/* Main PLL (NPRUN variant): 2 GHz VCO. DIVOUT1 reprogrammed to 666.67 MHz
 * (fracDiv[1].sel = 12 -> 2000/3.0) to feed NPU_ROOTCLK = 666 MHz in NPRUN.
 * DIVOUT0 and DIVOUT2 kept identical to HPRUN. In NPRUN CM85 sources from
 * COREPLL_OUT directly, so DIVOUT1 does not need to stay at 1000 MHz. */
static const clock_cguana_frac_pll_config_t s_mainPllConfig_np = {
    .startMode = kCLOCK_CguanaPllStartFull,
    .refFreq   = kCLOCK_CguanaRefFreq24M,
    .lowFreq   = kCLOCK_CguanaFracPllVco2000M,
    .div5En    = true,
    .div8En    = true,
    .div10En   = true,
    .div20En   = true,
    .fracDiv = {
            {.en = true, .range = false, .sel = 12U}, /* DIVOUT0: 2000/3.0  = 666.67 MHz */
            {.en = true, .range = false, .sel = 12U}, /* DIVOUT1: 2000/3.0  = 666.67 MHz -- NPRUN NPU */
            {.en = true, .range = false, .sel = 10U}, /* DIVOUT2: 2000/2.5  = 800 MHz    */
    },
    .sscgEn = false,
    .sscg   = NULL,
};

/* Main PLL (LPRUN variant): 2 GHz VCO. DIVOUT1 reprogrammed to 400 MHz
 * (fracDiv[1].sel = 20 -> 2000/5.0) to feed NPU_ROOTCLK = 400 MHz in LPRUN.
 * DIVOUT0 and DIVOUT2 kept identical to HPRUN. */
static const clock_cguana_frac_pll_config_t s_mainPllConfig_lp = {
    .startMode = kCLOCK_CguanaPllStartFull,
    .refFreq   = kCLOCK_CguanaRefFreq24M,
    .lowFreq   = kCLOCK_CguanaFracPllVco2000M,
    .div5En    = true,
    .div8En    = true,
    .div10En   = true,
    .div20En   = true,
    .fracDiv = {
            {.en = true, .range = false, .sel = 12U}, /* DIVOUT0: 2000/3.0  = 666.67 MHz */
            {.en = true, .range = false, .sel = 20U}, /* DIVOUT1: 2000/5.0  = 400 MHz    -- LPRUN NPU */
            {.en = true, .range = false, .sel = 10U}, /* DIVOUT2: 2000/2.5  = 800 MHz    */
    },
    .sscgEn = false,
    .sscg   = NULL,
};

/* Sys PLL: 2 GHz VCO, integer and fractional outputs enabled. */
static const clock_cguana_frac_pll_config_t s_sysPllConfig = {
    .startMode = kCLOCK_CguanaPllStartFull,
    .refFreq   = kCLOCK_CguanaRefFreq24M,
    .lowFreq   = kCLOCK_CguanaFracPllVco2000M,
    .div5En    = true, /* 400 MHz -- feeds SYSPLL_DIV5 -> SYSPLLDIVX/SYSPLLDIV5 route */
    .div8En    = true, /* 250 MHz */
    .div10En   = true, /* 200 MHz */
    .div20En   = true, /* 100 MHz */
    .fracDiv = {
            {.en = true, .range = false, .sel = 8U},  /* DIVOUT0: 2000/2.0  = 1000 MHz */
            {.en = true, .range = false, .sel = 8U},  /* DIVOUT1: 2000/2.0  = 1000 MHz */
            {.en = true, .range = false, .sel = 16U}, /* DIVOUT2: 2000/4.0  = 500 MHz  */
    },
    .sscgEn = false,
    .sscg   = NULL,
};

/* Audio PLL: F_OUT = 49.152 MHz (48 kHz audio family). Per RM 117.4.4.1 audio-mode algorithm:
 *   F_AVPLL    = 16 x 49.152 = 786.432 MHz
 *   POSTDIV    = 16
 *   dBAND      = (786.432 - 722.5344) / 6 = 10.6496 MHz
 *   CCO band   = ROUND((786.432 - 722.5344) / dBAND) = 6  (Table 802 row 110b: 786.432 MHz)
 *   DNUM       = 0  (audio mode: no fractional offset; LOOPDIV auto-computed via lookup) */
static const clock_cguana_avpll_config_t s_audioPllConfig = {
    .startMode    = kCLOCK_CguanaPllStartFull,
    .refFreq      = kCLOCK_CguanaRefFreq24M,
    .ccoBandSel   = 6U,
    .postDivRatio = 16U,
    .dnum         = 0U,
    .sscgEn       = false,
    .sscg         = NULL,
};

/* Video PLL: F_OUT = 70.64 MHz (display panel reference). Per RM 117.4.5.1 video-mode algorithm:
 *   POSTDIV    = ROUND(759.808 / 70.64)        = 11
 *   F_AVPLL    = 70.64 x 11                    = 777.04 MHz
 *   CCO band   = ROUND((777.04 - 722.5344) / dBAND) = 5  (Table 802 row 101b: 775.7824 MHz)
 *   F_VCO_CAL  = 5 x 10.6496 + 722.5344        = 775.7824 MHz
 *   LOOPDIV    = floor(F_VCO_CAL / 24)         = 32  (auto-determined by PLL lookup table)
 *   D_NUM      = (777.04 / 24) - 32            = 113/300 ~= 0.376667
 *   DNUM[29:0] = ROUND(D_NUM x 2^30)            = 404,442,754 = 0x181B4E82
 * Sanity-check: 24 x (32 + 0x181B4E82 / 2^30) / 11 = 70.640000 MHz OK */
static const clock_cguana_avpll_config_t s_videoPllConfig = {
    .startMode    = kCLOCK_CguanaPllStartFull,
    .refFreq      = kCLOCK_CguanaRefFreq24M,
    .ccoBandSel   = 5U,
    .postDivRatio = 11U,
    .dnum         = 0x181B4E82U, /* (113/300) x 2^30 = 404,442,754 -> 70.64 MHz */
    .sscgEn       = false,
    .sscg         = NULL,
};

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
    POWER_EnterHpRun(BOARD_BootClockHPRUN);
}

/*******************************************************************************
 ******************* Configuration BOARD_BootClockHP/NP/LPRUN ******************
 ******************************************************************************/
/* Static helper prototypes -- see definitions after the entry functions. */
static void ConfigCGUAna(void);
/* CGUDig is decomposed as: one shared block that programs every CCM slice that is
 * identical across HP/NP/LP (MODCON, PLL divider roots, PERI0-7, ETH, AUDIOBUS,
 * COMMBUS, WAKEBUS, CMPT, MAIN, MEDIA, AUDIO, COMM, WAKE domain slices) + one
 * per-mode block that programs the three CGU slices whose source/divider differ
 * between the three modes (CGU_MAIN_ROOTCLK slice 30, CGU_NPU_ROOTCLK slice 31,
 * CGU_MEDIABUS_ROOTCLK slice 32). Each BOARD_BootClock*RUN entry calls the shared
 * block followed by its own per-mode block. */
static void ConfigCGUDig_SS(void);
static void ConfigCGUDig_SYSCON_common(void);
static void ConfigCGUDig_SYSCON_HPRUN(void);
static void ConfigCGUDig_SYSCON_NPRUN(void);
static void ConfigCGUDig_SYSCON_LPRUN(void);
static void ConfigCGUDig_CMPT(void);
static void ConfigCGUDig_MAIN(void);
static void ConfigCGUDig_MEDIA(void);
static void ConfigCGUDig_AUDIO(void);
static void ConfigCGUDig_COMM(void);
static void ConfigCGUDig_WAKE(void);

/* XSPI0 boot flash runs XIP, so the CPU is fetching instructions over XSPI0 while
 * BOARD_BootClock*RUN reprograms the whole clock tree. Out of boot ROM, XSPI0 (MAIN
 * CCM CLOCK_ROOT[1], mux0) is sourced from:
 *     XSPI0 <- MAIN_PERI0_DIV2 <- PERI_ROOTCLK0 <- Main PLL DIVOUT0
 * Reprogramming that branch (PERI_ROOTCLK0 in ConfigCGUDig_SYSCON_common, or the
 * Main PLL) tears down the flash clock and crashes XIP. To avoid that, we first hop
 * XSPI0 onto the already-locked Sys PLL (mux2 = SYSPLLDIV4_ROOTCLK = SysPLL DIV4,
 * 500 MHz) divided down to 100 MHz. After the hop XSPI0 no longer depends on the
 * Main PLL / PERI0 branch, so the rest of the tree can be programmed from flash.
 *
 * This helper MUST be RAM-resident: while the root mux switches, the CPU keeps
 * fetching from XSPI0 flash, so the code performing the switch cannot itself live in
 * that flash. AT_QUICKACCESS_SECTION_CODE places it in the (SRAM) quickaccess
 * section, copied by startup.
 *
 * Frequency note: this LOWERS XSPI0 from its ROM operating point (~120-133 MHz) to
 * 100 MHz. A frequency decrease with unchanged controller/LUT/sampling needs no DLL
 * retrain -- the read data eye only widens and loopback-from-DQS sampling is
 * source-synchronous. If you later RAISE the XSPI0 frequency or move to DDR, you MUST
 * relocate fsl_xspi.o into RAM and call XSPI_SoftwareReset()/XSPI_UpdateDllValue()
 * here after the mux switch, otherwise reads at the new frequency will corrupt. */
AT_QUICKACCESS_SECTION_CODE(static void BOARD_MoveXspi0ToSysPllDiv4(void))
{
    uint32_t v   = MAIN__CCM->CLOCK_ROOT[1].SLICE_CONTROL;

    /* Glitchless CCM root mux hop: both the old source (Main PLL/PERI0) and the new
     * source (SysPLL DIV4 = 500 MHz) are running at this point, so the switch is clean.
     * Set MUX + DIV + SND_DIV together (all -1 encoded): the XSPI0 slice has two
     * dividers (DIV -> 2x internal, SND_DIV -> SCK); leaving SND_DIV at the ROM value
     * while changing DIV breaks their ratio. mux2 = SYSPLLDIV4, div=5 -> 100 MHz,
     * sndDiv=1 (intended XSPI0 NOR SDR config). */
    v &= ~(CCM_SLICE_CONTROL_MUX_MASK | CCM_SLICE_CONTROL_DIV_MASK | CCM_SLICE_CONTROL_SND_DIV_MASK);
    v |= CCM_SLICE_CONTROL_MUX(2U) | CCM_SLICE_CONTROL_DIV(5U - 1U) | CCM_SLICE_CONTROL_SND_DIV(1U - 1U);
    MAIN__CCM->CLOCK_ROOT[1].SLICE_CONTROL = v;

    __DSB();
    __ISB();
    (void)MAIN__CCM->CLOCK_ROOT[1].SLICE_CONTROL; /* CM85 posted-write read-back flush */
}

/* XSPI1 PSRAM protection -- mirror of BOARD_MoveXspi0ToSysPllDiv4, for the DDR PSRAM.
 *
 * Out of boot ROM the PSRAM (XSPI1, MAIN CCM CLOCK_ROOT[2]) is memory-mapped/active and
 * clocked from Main PLL (mux0 = MAIN_PERI1_DIV2 <- PERI_ROOTCLK1 <- MAINPLL_DIVOUT1) --
 * the same PLL XSPI0 rides. Re-initialising Main PLL (CLOCK_InitMainPll) would glitch
 * MAINPLL_DIVOUT1 -> PERI1 -> XSPI1, corrupting the live DDR PSRAM (and instantly killing
 * psram_txt, which executes from it). So, exactly like XSPI0, hop XSPI1 onto SYSPLLDIV4
 * (SysPLL DIV4 = 500 MHz -- a stable clock, independent of Main PLL) BEFORE Main PLL is
 * re-initialised. We copy ROM's dividers exactly (DIV÷1 -> 2x launch = 500 MHz, SND_DIV÷2
 * -> SCK = 250 MHz), which matches ROM because SYSPLLDIV4 (500) == ROM's MAIN_PERI1_DIV2
 * (500) -- so the PSRAM frequency is unchanged. Both the old (Main PLL) and new (Sys PLL
 * DIV4) sources are running at hop time, so the CCM root-mux switch itself is glitchless.
 *
 * The clock SOURCE changes (Main PLL -> Sys PLL), so even at the same 125 MHz SCK the DDR
 * read strobe the ROM DLL locked to is no longer aligned (single-beat / non-cacheable
 * reads fail). We therefore RE-LOCK the controller DLL for the new clock immediately after
 * the mux switch -- controller DLL only, NO Global Reset / device re-init, so the PSRAM
 * contents (incl. psram_txt code) are preserved.
 *
 * MUST be RAM-resident: for psram_txt (code executes from PSRAM) the CPU is fetching over
 * XSPI1 while the mux switches AND while the DLL re-locks (module briefly MDIS'd), so this
 * whole routine cannot live in that PSRAM. */
AT_QUICKACCESS_SECTION_CODE(static void BOARD_MoveXspi1ToSysPllDiv4(void))
{
    uint32_t   v    = MAIN__CCM->CLOCK_ROOT[2].SLICE_CONTROL;
    uint32_t   i;

    /* Match ROM's XSPI1 dividers EXACTLY so the PSRAM frequency (and thus the ROM DLL
     * calibration) is preserved. ROM: source MAIN_PERI1_DIV2 = 500 MHz, DIV(/1) -> 2x =
     * 500 MHz, SND_DIV(/2) -> SCK = 250 MHz (ROM SLICE_CONTROL = 0x00010000). Our source
     * SYSPLLDIV4 is also 500 MHz, so we copy DIV(/1)/SND_DIV(/2) and only change the mux
     * to 2 (SYSPLLDIV4). DIV/SND_DIV are (value-1) encoded. (An earlier version used
     * DIV/2, which HALVED the clock to 2x=250/SCK=125 and de-tuned the DDR DLL.) */
    v &= ~(CCM_SLICE_CONTROL_MUX_MASK | CCM_SLICE_CONTROL_DIV_MASK | CCM_SLICE_CONTROL_SND_DIV_MASK);
    v |=  CCM_SLICE_CONTROL_MUX(2U) | CCM_SLICE_CONTROL_DIV(1U - 1U) | CCM_SLICE_CONTROL_SND_DIV(2U - 1U);
    MAIN__CCM->CLOCK_ROOT[2].SLICE_CONTROL = v;

    __DSB();
    __ISB();
    (void)MAIN__CCM->CLOCK_ROOT[2].SLICE_CONTROL; /* CM85 posted-write read-back flush */

    /* Re-lock the DDR read DLL for the new clock -- ONLY if XSPI1 actually holds a live
     * DDR PSRAM that ROM already brought up (MCR.X16_EN set). Runtime gate, not a build
     * macro: in a build with no live PSRAM X16_EN is clear and we skip (a DDR DLL re-lock
     * on a non-DDR XSPI1 would be wrong). Enable the XSPI1 LPCG first so the MCR read
     * cannot bus-fault when the module clock happens to be off.
     *
     * Values are the validated ones from BOARD_ConfigXspiForPsram (DLLCR[0]=0xC260001C =
     * DLLEN|FREQEN|REFCNTR2|RES6|CDL8|AUTO_UPD|SLV_EN, SMPR=0x04000000). The SDK
     * XSPI_UpdateDllValue is deliberately NOT used: at 125 MHz (< 130 MHz auto-update
     * threshold) it drops FREQEN and never sets CDL8, producing a different DLLCR than the
     * working state. MDIS brackets the DLL reprogram; DLLCR is zeroed first to force a
     * fresh lock at the new clock. No Global Reset -> device data preserved. Bounded wait
     * so a dead board cannot hang boot. */
    CLOCK_EnableClock(kCLOCK_MAIN_xspi1);
    if ((MAIN__XSPI_1->MCR & XSPI_MCR_X16_EN_MASK) != 0U)
    {
        MAIN__XSPI_1->MCR |= XSPI_MCR_MDIS_MASK;
        MAIN__XSPI_1->DLLCR[0] = 0U;                     /* drop the stale (Main-PLL-clock) lock */
        MAIN__XSPI_1->DLLCR[0] = 0xC260001CU;            /* re-arm auto-DLL for the Sys-PLL clock */
        for (i = 0U; ((MAIN__XSPI_1->DLLSR & XSPI_DLLSR_SLVA_LOCK_MASK) == 0U) && (i < 100000U); i++)
        {
        }
        MAIN__XSPI_1->SMPR  = 0x04000000U;               /* DLLFSMPFA tap = 4 (working-state value) */
        MAIN__XSPI_1->MCR  &= ~XSPI_MCR_MDIS_MASK;
        MAIN__XSPI_1->MCR  |= XSPI_MCR_SWRSTSD_MASK;     /* serial soft-reset pulse (working-state) */
        MAIN__XSPI_1->MCR  &= ~XSPI_MCR_SWRSTSD_MASK;
    }

    __DSB();
    __ISB();
}

/* Select SXOSC as the OSC_24M (L0) source (MODCON CLK24M_SEL.SEL: 0 = FRO24M reset
 * default, 1 = SXOSC). This re-references the running CMS PLLs onto the crystal; the
 * XSPI0 XIP flash rides one of those PLLs out of boot ROM, so the switch can jitter
 * the flash clock. Do it from RAM with a direct register write so the CPU is not
 * fetching from XSPI0 flash across the transient.
 *
 * Equivalent to MODCON_SetCFG(kModCon_MAIN_CLK24M_SEL, 0, 0x1); the target register is
 * MAIN__MODCON->IP[getModConOffset(kModCon_MAIN_CLK24M_SEL)].CFG[0], a compile-time
 * constant address (no flash .rodata dependency). SXOSC must already be running
 * (CLOCK_InitSxosc) before this is called. */
AT_QUICKACCESS_SECTION_CODE(static void BOARD_SetOsc24mSxoscFromRam(void))
{
    MAIN__MODCON->IP[getModConOffset((uint32_t)kModCon_MAIN_CLK24M_SEL)].CFG[0] = 0x1U;

    __DSB();
    __ISB();
    (void)MAIN__MODCON->IP[getModConOffset((uint32_t)kModCon_MAIN_CLK24M_SEL)].CFG[0];
}

/* Park the CM85 core on FRO192M, then switch OSC_24M to SXOSC -- both from RAM.
 *
 * Switching the CMS PLL reference (MODCON CLK24M_SEL: FRO24M -> SXOSC) glitches the PLL
 * outputs during the transition. Out of boot ROM the CM85 core (CGU MAIN_ROOTCLK,
 * slice 30) rides Main PLL, so that glitch can crash the core. FRO192M is a free-running
 * on-chip oscillator, independent of the OSC24M/SXOSC reference and of every PLL, so
 * parking the core on it (MAIN mux0 = BASE = FRO_192M) makes the core immune both to the
 * reference switch here AND to the later Main PLL re-init (CLOCK_InitMainPll). The core
 * stays on FRO192M until ConfigCGUDig_SYSCON_*RUN moves it to its final PLL_PFDX source,
 * which runs after Main PLL has re-locked (CLOCK_InitMainPll blocks until lock via the
 * CGUANA FSM RDY wait).
 *
 * Idempotent: if MAIN_ROOTCLK is already on BASE the park is skipped. Only the MUX field
 * is changed (div/sndDiv left as ROM set them). FRO192M runs out of ROM (it feeds the
 * FRO24M default OSC_24M source), and BASE_CLK is forced to FRO_192M just below, so both
 * the old (Main PLL) and new (FRO192M) sources are live -> the root-mux hop is glitchless.
 *
 * RAM-resident + direct register writes (no CLOCK_SetRootClock, whose lookup tables live
 * in XSPI0 flash) so the CPU is not fetching from flash across the transient. */
AT_QUICKACCESS_SECTION_CODE(static void BOARD_ParkCoreAndSwitchOsc24m(void))
{
    uint32_t v;

    /* Guarantee the park target is live: force BASE_CLK -> FRO_192M (mux only). BASE_CLK
     * feeds nothing the running core/flash depend on here, so this write is safe. */
    v  = SYSCON__CCM->CLOCK_ROOT[kCLOCK_Root_CGU_BASE_CLK].SLICE_CONTROL;
    v &= ~CCM_SLICE_CONTROL_MUX_MASK;
    v |=  CCM_SLICE_CONTROL_MUX((uint32_t)kCLOCK_BASE_ClockRoot_FRO_192M);
    SYSCON__CCM->CLOCK_ROOT[kCLOCK_Root_CGU_BASE_CLK].SLICE_CONTROL = v;
    __DSB();
    __ISB();
    (void)SYSCON__CCM->CLOCK_ROOT[kCLOCK_Root_CGU_BASE_CLK].SLICE_CONTROL;

    v = SYSCON__CCM->CLOCK_ROOT[kCLOCK_Root_CGU_MAIN_ROOTCLK].SLICE_CONTROL;

    if ((v & CCM_SLICE_CONTROL_MUX_MASK) !=
        CCM_SLICE_CONTROL_MUX((uint32_t)kCLOCK_CGU_MAIN_ClockRoot_BASE))
    {
        v &= ~CCM_SLICE_CONTROL_MUX_MASK;
        v |=  CCM_SLICE_CONTROL_MUX((uint32_t)kCLOCK_CGU_MAIN_ClockRoot_BASE);
        SYSCON__CCM->CLOCK_ROOT[kCLOCK_Root_CGU_MAIN_ROOTCLK].SLICE_CONTROL = v;

        __DSB();
        __ISB();
        (void)SYSCON__CCM->CLOCK_ROOT[kCLOCK_Root_CGU_MAIN_ROOTCLK].SLICE_CONTROL;
    }

    /* Core now on FRO192M -- safe to re-reference the CMS PLLs onto the crystal. */
    BOARD_SetOsc24mSxoscFromRam();
}

#if !(defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1))
/* Shared, mode-invariant prologue for HP/NP/LP boot-clock setup -- steps 1..4 of the
 * hazard-ordered flow. Takes NO parameters: everything it touches is identical across
 * the three run modes, so it references the file-scope s_*Config globals directly.
 *
 * The per-mode tail (Core PLL, Main PLL, then the CGUDig tree) is done by each
 * BOARD_BootClock*RUN entry AFTER this returns -- those steps use the per-mode config
 * globals by name, so nothing has to be threaded through this helper.
 *
 *   1) Park the CM85 core on FRO192M + switch OSC_24M -> SXOSC (RAM-resident). The
 *      reference switch glitches every CMS PLL; the core rides Main PLL out of boot ROM,
 *      so it must be off ALL PLLs before the switch. It stays on FRO192M until the
 *      per-mode CGUDig tail moves it to its final source (after Main PLL has re-locked).
 *   2) Bring up the mode-invariant analog blocks (ConfigCGUAna): FROs, SXOSC, Sys/Audio/
 *      Video PLL. Core PLL and Main PLL are DEFERRED -- XSPI0 XIP flash / XSPI1 PSRAM
 *      still ride Main PLL out of ROM, and both PLLs only need to be locked just before
 *      the CGUDig tree, which the per-mode tail owns.
 *   3) Bring up SYSPLLDIV4_ROOTCLK (SysPLL DIV4 = 500 MHz) and hop XSPI0 onto it
 *      (RAM-resident, 100 MHz). div MUST match ConfigCGUDig_SYSCON_common()'s
 *      SYSPLLDIV4_ROOTCLK setting (div=1 -> 500 MHz): common() re-programs this root
 *      AFTER the hop, while XSPI0 is riding it; a different div here would re-divide
 *      XSPI0's clock live (from flash) and corrupt XIP.
 *   4) Hop XSPI1 (DDR PSRAM) onto SYSPLLDIV4 too, incl. controller DLL re-lock. */
static void BOARD_BootClockPrepare(void)
{
    clock_root_config_t config = {.mux = kCLOCK_LPUART0_ClockRoot_SXOSC, .div = 1,};
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpuart0_fclk, &config);

    /* Step 1: park core on FRO192M + OSC_24M -> SXOSC, both from RAM. */
    BOARD_ParkCoreAndSwitchOsc24m();

    /* Step 2: mode-invariant analog bring-up (FROs, SXOSC, Sys/Audio/Video PLL).
     * Core PLL + Main PLL stay DEFERRED -- the per-mode entry inits them next. */
    ConfigCGUAna();

    /* Step 3: XSPI0 (boot flash, XIP) -> SysPLL DIV4. */
    
    clock_root_config_t sysPllDiv4Cfg = {
        .mux    = kCLOCK_SYSPLLDIV4_ClockRoot_SYSPLL_DIV4,
        .div    = 1U,
        .sndDiv = 1U,
    };
    CLOCK_SetRootClock(kCLOCK_Root_CGU_SYSPLLDIV4_ROOTCLK, &sysPllDiv4Cfg);

    BOARD_MoveXspi0ToSysPllDiv4();

    /* Step 4: XSPI1 (DDR PSRAM) -> SysPLL DIV4 (+ DLL re-lock). */
    BOARD_MoveXspi1ToSysPllDiv4();
}
#endif /* !RT2660_PRESILICON_DEVELOPMENT */

/* Over Drive Run FBB (HpRun): CM85=1000 (via PLL_PFDX), NPU=792 (from COREPLL_OUT).
 * CorePLL @ 792 MHz, MainPLL DIVOUT1 @ 1000 MHz -- the default analog config. */
void BOARD_BootClockHPRUN(void)
{
#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
    /* Pre-silicon (default): the HAPS / RTL environment does NOT support the
     * CCM/CGUDig path; running the helpers would derail validation. The (void)
     * casts keep the helpers and per-mode PLL configs code-reviewed and
     * referenced (so -Wunused-function / -Wunused-const-variable stay quiet),
     * without emitting any code at runtime. */
    (void)ConfigCGUAna;
    (void)ConfigCGUDig_SYSCON_common;
    (void)ConfigCGUDig_SYSCON_HPRUN;
    (void)ConfigCGUDig_SS;

    (void)&s_corePllConfig;
    (void)&s_mainPllConfig;
#else
    BOARD_BootClockPrepare();
    /* Per-mode tail -- config globals used directly. Both PLLs must be locked before
     * ConfigCGUDig_SYSCON_common() (it sources PLL_PFDX <- MAINPLL_DIVOUT1 and
     * COMMPFDX <- COREPLL_OUT). */
    CLOCK_InitCorePll(&s_corePllConfig);           /* 792 MHz */
    CLOCK_InitMainPll(&s_mainPllConfig);           /* DIVOUT1 = 1000 MHz */
    ConfigCGUDig_SYSCON_common();                  /* CGU slices identical across modes */
    ConfigCGUDig_SYSCON_HPRUN();                   /* CGU slices 30/31/32 for HpRun */
    ConfigCGUDig_SS();                      /* CMPT / MAIN / MEDIA / AUDIO / COMM / WAKE */
#endif
}

/* Normal Drive Run FBB (NpRun): CM85=792 (from COREPLL_OUT), NPU=666 (from MAINPLL_DIVOUT1).
 * CorePLL @ 792 MHz (shared with HPRUN), MainPLL DIVOUT1 @ 666.67 MHz (per-mode variant).
 * CGU slices 30/31/32 are re-programmed after the shared CGUDig defaults. */
void BOARD_BootClockNPRUN(void)
{
#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
    (void)ConfigCGUAna;
    (void)ConfigCGUDig_SYSCON_common;
    (void)ConfigCGUDig_SYSCON_NPRUN;
    (void)ConfigCGUDig_SS;
    (void)&s_corePllConfig;
    (void)&s_mainPllConfig_np;
#else
    BOARD_BootClockPrepare();
    CLOCK_InitCorePll(&s_corePllConfig);           /* 792 MHz (shared with HpRun) */
    CLOCK_InitMainPll(&s_mainPllConfig_np);        /* DIVOUT1 = 666.67 MHz */
    ConfigCGUDig_SYSCON_common();                  /* CGU slices identical across modes */
    ConfigCGUDig_SYSCON_NPRUN();                   /* CGU slices 30/31/32 for NpRun */
    ConfigCGUDig_SS();                      /* CMPT / MAIN / MEDIA / AUDIO / COMM / WAKE */
#endif
}

/* Normal Drive Run ZBB (LpRun): CM85=600 (from COREPLL_OUT), NPU=400 (from MAINPLL_DIVOUT1).
 * CorePLL @ 600 MHz (per-mode variant), MainPLL DIVOUT1 @ 400 MHz (per-mode variant).
 * Caller must have DCDC=0.8V and FBB disabled before invoking (not enforced here). */
void BOARD_BootClockLPRUN(void)
{
#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
    (void)ConfigCGUAna;
    (void)ConfigCGUDig_SYSCON_common;
    (void)ConfigCGUDig_SYSCON_LPRUN;
    (void)ConfigCGUDig_SS;
    (void)&s_corePllConfig_lp;
    (void)&s_mainPllConfig_lp;
#else
    BOARD_BootClockPrepare();
    CLOCK_InitCorePll(&s_corePllConfig_lp);        /* 600 MHz */
    CLOCK_InitMainPll(&s_mainPllConfig_lp);        /* DIVOUT1 = 400 MHz */
    ConfigCGUDig_SYSCON_common();                  /* CGU slices identical across modes */
    ConfigCGUDig_SYSCON_LPRUN();                   /* CGU slices 30/31/32 for LpRun */
    ConfigCGUDig_SS();                      /* CMPT / MAIN / MEDIA / AUDIO / COMM / WAKE */
#endif
}

/* Deprecated alias for BOARD_BootClockHPRUN -- kept for source-compat with any
 * out-of-tree caller. New code should call BOARD_BootClockHPRUN() directly. */
void BOARD_BootClockRUN(void)
{
    BOARD_BootClockHPRUN();
}

/*******************************************************************************
 * Static helpers
 ******************************************************************************/

/* Bring up all mode-invariant CGUAna analog blocks: FRO12M, FRO192M, SXOSC, and the
 * Sys / Audio / Video PLLs. These are IDENTICAL across HPRUN / NPRUN / LPRUN, so this
 * function takes no parameters and reads the file-scope s_*Config globals directly.
 *
 * The two per-mode PLLs are NOT initialised here -- each BOARD_BootClock*RUN entry does
 * that itself, referencing its own config globals by name:
 *   - Core PLL differs per mode (792 MHz HP/NP, 600 MHz LP), so CLOCK_InitCorePll is
 *     called by the per-mode entry after BOARD_BootClockPrepare() returns.
 *   - Main PLL is deferred: XSPI0 XIP flash still rides it out of boot ROM, so it is
 *     initialised by the per-mode entry only after XSPI0/XSPI1 are hopped onto Sys PLL. */
static void ConfigCGUAna(void)
{
    // TODO, LPOSC_12M, LPOSC_1M and LPOSC_32K

    /* PLL reference clock source selection (MCUX-88628). Must be programmed before
     * the PLLs are initialized. This board routes both the Core/Main/Sys (CMS)
     * PLLs and the Audio/Video (AV) PLLs from the SXOSC 24 MHz crystal -- matches
     * the OSC_24M MODCON routing applied in ConfigCGUDig_SYSCON_common. */
    CLOCK_SetCmsPllRefSource(kCLOCK_PllRefSrc_SXOSC);
    CLOCK_SetAvPllRefSource(kCLOCK_PllRefSrc_SXOSC);

    /* FRO 12 MHz -- early safe fallback; brought up first so the rest of the analog
     * bring-up always has a known-good clock available. */
    CLOCK_InitFro12M(&s_fro12mConfig);

    /* FRO 192 MHz -- makes FRO-based root clocks available immediately. */
    CLOCK_InitFro192M(&s_fro192mConfig);

    /* SXOSC 24 MHz crystal -- used as PLL reference. */
    CLOCK_InitSxosc(&s_sxoscConfig);

    /* Sys PLL -> 2 GHz VCO; secondary fixed and fractional outputs.
     * Re-initialised in all builds: PSRAM (XSPI1) rides Main PLL out of ROM (mux0 =
     * MAIN_PERI1_DIV2 <- PERI_ROOTCLK1 <- MAINPLL_DIVOUT1), NOT Sys PLL, so re-locking
     * Sys PLL here does not disturb the live PSRAM. A freshly-locked Sys PLL (DIV4 = 500)
     * is the deterministic hop target for both XSPI0 and XSPI1. */
    CLOCK_InitSysPll(&s_sysPllConfig);

    /* Audio PLL -> 49.152 MHz (feeds AUDIOPLL_ROOTCLK; consumed by audio peripherals). */
    CLOCK_InitAudioPll(&s_audioPllConfig);

    /* Video PLL -> 70.64 MHz (feeds VIDEOPLL_ROOTCLK; consumed by the display path). */
    CLOCK_InitVideoPll(&s_videoPllConfig);
}

/* Program all CGUDig clock-root mux/divider slices, grouped by domain. One mutable
 * rootCfg is reused across the 7 domain helpers. sndDiv = 1 is set once here as a
 * baseline: CLOCK_SetRootClock writes both div and sndDiv to the CCM SLICE_CONTROL
 * register using a (value-1) encoding, so a zero-initialised sndDiv would write
 * 0xFFFFFFFF into the SND_DIV bitfield and corrupt the secondary divider on any
 * root that uses it. */
/* Shared CGUDig block: programs every CCM domain slice that is IDENTICAL across
 * HPRUN / NPRUN / LPRUN. The CGU per-mode slices (30 MAIN_ROOTCLK, 31 NPU_ROOTCLK,
 * 32 MEDIABUS_ROOTCLK) are programmed separately by ConfigCGUDig_CGU_HP/NP/LPRUN
 * -- see the BOARD_BootClock*RUN entry functions above.
 *
 * Note: the CMPT / MAIN / MEDIA domain slices track their upstream CGU roots
 * via mux=MAIN/NPU/MEDIABUS with div!=1 (e.g. CMPT.main_clk_divided = MAIN/3),
 * so they automatically follow whatever the per-mode CGU root is set to -- no
 * per-mode variant of CMPT/MAIN/MEDIA is needed. */
static void ConfigCGUDig_SS(void)
{
    ConfigCGUDig_MEDIA();
    ConfigCGUDig_AUDIO();
    ConfigCGUDig_CMPT();
    ConfigCGUDig_MAIN();
    ConfigCGUDig_COMM();
    ConfigCGUDig_WAKE();
}

/*
 * CGU domain clock roots (49 slices on SYSCON_CCM):
 *
 *  Slice  Root Name              Mux  Source            Div  Yield
 *  -----  --------------------  ---  ---------------  ---  ----------------
 *     1   BASE_CLK                0  FRO_192M           1  192 MHz
 *     2   LOW_CLK                 0  FRO_24M            1  24 MHz
 *     3   MAINPLL_DIVX            1  MAINPLL_DIV5       1  400 MHz
 *     4   SYSPLL_DIVX             1  SYSPLL_DIV5        1  400 MHz
 *     5   PLL_PFDX                0  MAINPLL_DIVOUT1    1  1000 MHz
 *     6   MEDIA_PFDX              2  SYSPLL_DIVOUT1     1  1000 MHz
 *     7   MAINPFDX_ROOTCLK        2  SYSPLL_DIVOUT1     1  1000 MHz
 *     8   COMMPFDX_ROOTCLK        0  COREPLL_OUT        1  792 MHz
 *     9   MAINDIVX_ROOTCLK        1  MAINPLL_DIV5       1  400 MHz
 *    10   SAIMCLK_ROOTCLK         1  SAI1_MCLK          1  ext (24 MHz typ)
 *    11   SAIMCLK0_ROOTCLK        0  SAI0_MCLK          1  ext (24 MHz typ)
 *    12   SAIMCLK1_ROOTCLK        0  SAI1_MCLK          1  ext (24 MHz typ)
 *    13   SAIMCLK2_ROOTCLK        0  SAI2_MCLK          1  ext (24 MHz typ)
 *     0   SXOSC_ROOTCLK           0  OSC_24M            1  24 MHz
 *    14   LP12M_CORE_ROOTCLK      0  LPOSC_12M_CORE     1  12 MHz
 *    15   LP1M_CORE_ROOTCLK       0  LPOSC_1M_CORE      1  1 MHz
 *    16   ULP32K_ROOTCLK          0  LPOSC32K           1  32 kHz
 *    17   FRO192M_ROOTCLK         0  FRO_192M           1  192 MHz
 *    18   FRO96M_ROOTCLK          0  FRO_96M            1  96 MHz
 *    19   FRO48M_ROOTCLK          0  FRO_48M            1  48 MHz
 *    20   FRO24M_ROOTCLK          0  FRO_24M            1  24 MHz
 *    21   SYSPLLDIV4_ROOTCLK      0  SYSPLL_DIV4        1  500 MHz
 *    22   SYSPLLDIV5_ROOTCLK      0  SYSPLL_DIV5        1  400 MHz
 *    23   SYSPLLDIVX_ROOTCLK      1  SYSPLL_DIV5        1  400 MHz
 *    24   MAINPLLDIVX_ROOTCLK     1  MAINPLL_DIV5       1  400 MHz
 *    25   MAINPLLDIV8_ROOTCLK     0  MAINPLL_DIV8       1  250 MHz
 *    26   MAINPLLDIV10_ROOTCLK    0  MAINPLL_DIV10      1  200 MHz
 *    27   MAINPLLDIV20_ROOTCLK    0  MAINPLL_DIV20      1  100 MHz
 *    28   AUDIOPLL_ROOTCLK        0  AUDIOPLL_DIVOUT    1  49.15 MHz
 *    29   VIDEOPLL_ROOTCLK        0  VIDEOPLL_DIVOUT    1  70.64 MHz
 *    30   MAIN_ROOTCLK            3  PLL_PFDX           1  1000 MHz   (sndDiv = 1; CMPT MAIN = div+sndDiv, CMPT CPU = div only)
 *    31   NPU_ROOTCLK             1  COREPLL_OUT        1  792 MHz
 *    32   MEDIABUS_ROOTCLK        3  PLL_PFDX           3  333.33 MHz
 *    33   AUDIOBUS_ROOTCLK        2  MAINPLL_DIVX       2  200 MHz
 *    34   COMMBUS_ROOTCLK         2  MAINPLL_DIVX       2  200 MHz
 *    35   WAKEBUS_ROOTCLK         2  MAINPLL_DIVX      10  40 MHz
 *    36   SYSCON_PDMAIN_CLK       3  SYSPLL_DIV10       1  200 MHz
 *    37   PERI_ROOTCLK0           1  MAINPLL_DIVOUT0    1  666.67 MHz
 *    38   PERI_ROOTCLK1           1  MAINPLL_DIVOUT1    1  1000 MHz
 *    39   PERI_ROOTCLK2           1  MAINPLL_DIVOUT2    2  400 MHz
 *    40   PERI_ROOTCLK3           2  MAINPLL_DIVX       1  400 MHz
 *    41   PERI_ROOTCLK4           2  MAINPLL_DIVX       1  400 MHz
 *    42   PERI_ROOTCLK5           2  MAINPLL_DIVX       1  400 MHz
 *    43   PERI_ROOTCLK6           0  BASE               8  24 MHz
 *    44   PERI_ROOTCLK7           1  FRO_192M           5  38.4 MHz
 *    45   AUDIO_ROOTCLK           0  LOW                1  24 MHz
 *    46   VIDEO_ROOTCLK           0  BASE               1  192 MHz
 *    47   USB1_ROOTCLK            0  FRO_48M            1  48 MHz
 *    48   ETH_ROOTCLK             3  SYSPLL_DIV20       1  100 MHz
 */
static void ConfigCGUDig_SYSCON_common(void)
{
    clock_root_config_t rootCfg = {0};
    rootCfg.sndDiv              = 1U;

    /* MCUX-88602 MODCON-controlled clock-tree muxes.
     *
     * OSC_24M (L0): route to the SXOSC crystal (PLL reference) -- SXOSC has
     * already been brought up by InitCguAna()/CLOCK_InitSxosc().
     *
     * The six Mx (L2) /2 selects feed XSPI0/XSPI1 (MAIN) and USDHC0/USDHC1
     * (COMM). Default them all to pass-through so consumers see the full
     * PERI/PFD root frequencies tabulated above. */
    /* OSC_24M is now switched to SXOSC earlier (from RAM, with the core parked on
     * FRO192M) by BOARD_ParkCoreAndSwitchOsc24m() in every BOARD_BootClock*RUN entry,
     * so the flash-resident switch here is redundant (CLK24M_SEL already = SXOSC). */
    /* CLOCK_SetOsc24mSource(kCLOCK_Osc24mSrc_SXOSC); */

     CLOCK_SetClockSrcDiv2(kCLOCK_SRC_MAINPFDX_DIV2,   false);
     CLOCK_SetClockSrcDiv2(kCLOCK_SRC_MAIN_PERI0_DIV2, false);
    CLOCK_SetClockSrcDiv2(kCLOCK_SRC_MAIN_PERI1_DIV2, false);
    CLOCK_SetClockSrcDiv2(kCLOCK_SRC_COMMPFDX_DIV2, false);
    CLOCK_SetClockSrcDiv2(kCLOCK_SRC_COMM_PERI1_DIV2, false);
    CLOCK_SetClockSrcDiv2(kCLOCK_SRC_COMM_PERI2_DIV2, false);

    rootCfg.mux = kCLOCK_BASE_ClockRoot_FRO_192M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_BASE_CLK, &rootCfg);

    rootCfg.mux = kCLOCK_LOW_ClockRoot_FRO_24M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_LOW_CLK, &rootCfg);

    rootCfg.mux = kCLOCK_MAINPLL_DIVX_ClockRoot_MAINPLL_DIV5;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MAINPLL_DIVX, &rootCfg);

    rootCfg.mux = kCLOCK_SYSPLL_DIVX_ClockRoot_SYSPLL_DIV5;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_SYSPLL_DIVX, &rootCfg);

    rootCfg.mux = kCLOCK_PLL_PFDX_ClockRoot_MAINPLL_DIVOUT1;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_PLL_PFDX, &rootCfg);

    rootCfg.mux = kCLOCK_MEDIA_PFDX_ClockRoot_SYSPLL_DIVOUT1;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MEDIA_PFDX, &rootCfg);

    rootCfg.mux = kCLOCK_MAINPFDX_ClockRoot_SYSPLL_DIVOUT1;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MAINPFDX_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_COMMPFDX_ClockRoot_COREPLL_OUT;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_COMMPFDX_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_MAINDIVX_ClockRoot_MAINPLL_DIV5;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MAINDIVX_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_SAIMCLK_ClockRoot_SAI1_MCLK;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_SAIMCLK_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_SAIMCLK0_ClockRoot_SAI0_MCLK;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_SAIMCLK0_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_SAIMCLK1_ClockRoot_SAI1_MCLK;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_SAIMCLK1_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_SAIMCLK2_ClockRoot_SAI2_MCLK;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_SAIMCLK2_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_SXOSC_ClockRoot_OSC_24M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_SXOSC_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_LP12M_CORE_ClockRoot_LPOSC_12M_CORE;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_LP12M_CORE_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_LP1M_CORE_ClockRoot_LPOSC_1M_CORE;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_LP1M_CORE_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_ULP32K_ClockRoot_LPOSC32K;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_ULP32K_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_FRO192M_ClockRoot_FRO_192M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_FRO192M_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_FRO96M_ClockRoot_FRO_96M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_FRO96M_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_FRO48M_ClockRoot_FRO_48M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_FRO48M_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_FRO24M_ClockRoot_FRO_24M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_FRO24M_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_SYSPLLDIV4_ClockRoot_SYSPLL_DIV4;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_SYSPLLDIV4_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_SYSPLLDIV5_ClockRoot_SYSPLL_DIV5;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_SYSPLLDIV5_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_SYSPLLDIVX_ClockRoot_SYSPLL_DIV5;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_SYSPLLDIVX_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_MAINPLLDIVX_ClockRoot_MAINPLL_DIV5;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MAINPLLDIVX_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_MAINPLLDIV8_ClockRoot_MAINPLL_DIV8;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MAINPLLDIV8_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_MAINPLLDIV10_ClockRoot_MAINPLL_DIV10;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MAINPLLDIV10_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_MAINPLLDIV20_ClockRoot_MAINPLL_DIV20;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MAINPLLDIV20_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_AUDIOPLL_ClockRoot_AUDIOPLL_DIVOUT;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_AUDIOPLL_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_VIDEOPLL_ClockRoot_VIDEOPLL_DIVOUT;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_VIDEOPLL_ROOTCLK, &rootCfg);

    /* CGU slices 30 (MAIN_ROOTCLK / CM85), 31 (NPU_ROOTCLK), 32 (MEDIABUS_ROOTCLK)
     * are programmed by the per-mode helpers ConfigCGUDig_CGU_HP/NP/LPRUN below --
     * their source/divider differ across HpRun / NpRun / LpRun. */

    rootCfg.mux = kCLOCK_AUDIOBUS_ClockRoot_MAINPLL_DIVX;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_AUDIOBUS_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_COMMBUS_ClockRoot_MAINPLL_DIVX;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_COMMBUS_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_WAKEBUS_ClockRoot_MAINPLL_DIVX;
    rootCfg.div = 10U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_WAKEBUS_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_SYSCON_PDMAIN_ClockRoot_SYSPLL_DIV10;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_SYSCON_PDMAIN_CLK, &rootCfg);

    rootCfg.mux = kCLOCK_PERI0_ClockRoot_MAINPLL_DIVOUT0;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_PERI_ROOTCLK0, &rootCfg);

    rootCfg.mux = kCLOCK_PERI1_ClockRoot_MAINPLL_DIVOUT1;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_PERI_ROOTCLK1, &rootCfg);

    rootCfg.mux = kCLOCK_PERI2_ClockRoot_MAINPLL_DIVOUT2;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_PERI_ROOTCLK2, &rootCfg);

    rootCfg.mux = kCLOCK_PERI3_ClockRoot_MAINPLL_DIVX;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_PERI_ROOTCLK3, &rootCfg);

    rootCfg.mux = kCLOCK_PERI4_ClockRoot_MAINPLL_DIVX;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_PERI_ROOTCLK4, &rootCfg);

    rootCfg.mux = kCLOCK_PERI5_ClockRoot_MAINPLL_DIVX;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_PERI_ROOTCLK5, &rootCfg);

    rootCfg.mux = kCLOCK_PERI6_ClockRoot_BASE;
    rootCfg.div = 8U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_PERI_ROOTCLK6, &rootCfg);

    rootCfg.mux = kCLOCK_PERI7_ClockRoot_FRO_192M;
    rootCfg.div = 5U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_PERI_ROOTCLK7, &rootCfg);

    rootCfg.mux = kCLOCK_AUDIO_ClockRoot_LOW;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_AUDIO_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_VIDEO_ClockRoot_BASE;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_VIDEO_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_USB1_ClockRoot_FRO_48M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_USB1_ROOTCLK, &rootCfg);

    rootCfg.mux = kCLOCK_ETH_ClockRoot_SYSPLL_DIV20;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_ETH_ROOTCLK, &rootCfg);
}

/* Per-mode CGU slices for HpRun (Over Drive Run FBB) -- Jira table column 1:
 *
 *  Slice 30  MAIN_ROOTCLK      PLL_PFDX          -> 1000 MHz  (CM85)
 *  Slice 31  NPU_ROOTCLK       COREPLL_OUT       -> 792 MHz   (NPU; spec-nominal 800)
 *  Slice 32  MEDIABUS_ROOTCLK  PLL_PFDX / 3      -> 333 MHz   (MEDIA)
 *
 * Downstream: CMPT.cpu_clk = MAIN_ROOTCLK/1 = 1000 (CM85 CPU clock);
 * CMPT.cmpt_clk = MAIN_ROOTCLK/1 with sndDiv=1 -> also 1000; MAIN.main_clk_divided
 * = MAIN_ROOTCLK/3 -> 333 MHz (MAIN bus / CMPT bus target). */
static void ConfigCGUDig_SYSCON_HPRUN(void)
{
    clock_root_config_t rootCfg = {0};
    rootCfg.sndDiv              = 1U;

    /* CGU ROOT 30 MAIN (renamed per MCUX-88628). Root 30 has two dividers:
     *   div    -> CPU_ROOTCLK  (feeds CMPT ROOT 1 cpu_clk / CM85 core)
     *   sndDiv -> MAIN_ROOTCLK (feeds CMPT ROOT 0 cmpt_clk / MAIN bus)
     * HpRun target: CM85 = 1000, MAIN bus = 333 MHz -> div=1, sndDiv=3. */
    rootCfg.mux    = kCLOCK_CGU_MAIN_ClockRoot_PLL_PFDX;
    rootCfg.div    = 1U;
    rootCfg.sndDiv = 3U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MAIN_ROOTCLK, &rootCfg);
    rootCfg.sndDiv = 1U;

    /* NPU_ROOTCLK: source COREPLL_OUT (= 792 MHz per s_corePllConfig). */
    rootCfg.mux = kCLOCK_NPU_ClockRoot_COREPLL_OUT;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_NPU_ROOTCLK, &rootCfg);

    /* MEDIABUS_ROOTCLK: source PLL_PFDX (=1000 MHz) / 3 = 333.33 MHz. */
    rootCfg.mux = kCLOCK_MEDIABUS_ClockRoot_PLL_PFDX;
    rootCfg.div = 3U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MEDIABUS_ROOTCLK, &rootCfg);
}

/* Per-mode CGU slices for NpRun (Normal Drive Run FBB) -- Jira table column 2:
 *
 *  Slice 30  MAIN_ROOTCLK      COREPLL_OUT       -> 792 MHz   (CM85; spec-nominal 800)
 *  Slice 31  NPU_ROOTCLK       MAINPLL_DIVOUT1   -> 666.67 MHz (NPU; needs s_mainPllConfig_np)
 *  Slice 32  MEDIABUS_ROOTCLK  SYSPLL_DIVOUT2/2  -> 250 MHz   (MEDIA)
 *
 * Downstream: CMPT.cpu_clk = MAIN_ROOTCLK/1 = 792 (CM85); MAIN.main_clk_divided
 * = MAIN_ROOTCLK/3 = 264 MHz (spec-nominal 266). */
static void ConfigCGUDig_SYSCON_NPRUN(void)
{
    clock_root_config_t rootCfg = {0};
    rootCfg.sndDiv              = 1U;

    /* MAIN_ROOTCLK: source COREPLL_OUT (= 792 MHz in NpRun). div=1 -> CM85 CPU
     * clock = 792 MHz; sndDiv=3 -> MAIN bus = 264 MHz (spec-nominal 266). */
    rootCfg.mux    = kCLOCK_CGU_MAIN_ClockRoot_COREPLL_OUT;
    rootCfg.div    = 1U;
    rootCfg.sndDiv = 3U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MAIN_ROOTCLK, &rootCfg);
    rootCfg.sndDiv = 1U;

    /* NPU_ROOTCLK: source MAINPLL_DIVOUT1 (= 666.67 MHz per s_mainPllConfig_np). */
    rootCfg.mux = kCLOCK_NPU_ClockRoot_MAINPLL_DIVOUT1;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_NPU_ROOTCLK, &rootCfg);

    /* MEDIABUS_ROOTCLK: source SYSPLL_DIVOUT2 (500 MHz) / 2 = 250 MHz. */
    rootCfg.mux = kCLOCK_MEDIABUS_ClockRoot_SYSPLL_DIVOUT2;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MEDIABUS_ROOTCLK, &rootCfg);
}

/* Per-mode CGU slices for LpRun (Normal Drive Run ZBB) -- Jira table column 3:
 *
 *  Slice 30  MAIN_ROOTCLK      COREPLL_OUT       -> 600 MHz   (CM85; per s_corePllConfig_lp)
 *  Slice 31  NPU_ROOTCLK       MAINPLL_DIVOUT1   -> 400 MHz   (NPU; needs s_mainPllConfig_lp)
 *  Slice 32  MEDIABUS_ROOTCLK  SYSPLL_DIVOUT2/2  -> 250 MHz   (MEDIA; same as NpRun)
 *
 * Downstream: CMPT.cpu_clk = 600 (CM85); MAIN.main_clk_divided = 600/3 = 200 MHz. */
static void ConfigCGUDig_SYSCON_LPRUN(void)
{
    clock_root_config_t rootCfg = {0};
    rootCfg.sndDiv              = 1U;

    /* MAIN_ROOTCLK: source COREPLL_OUT (= 600 MHz per s_corePllConfig_lp).
     * div=1 -> CM85 CPU clock = 600 MHz; sndDiv=3 -> MAIN bus = 200 MHz. */
    rootCfg.mux    = kCLOCK_CGU_MAIN_ClockRoot_COREPLL_OUT;
    rootCfg.div    = 1U;
    rootCfg.sndDiv = 3U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MAIN_ROOTCLK, &rootCfg);
    rootCfg.sndDiv = 1U;

    /* NPU_ROOTCLK: source MAINPLL_DIVOUT1 (= 400 MHz per s_mainPllConfig_lp). */
    rootCfg.mux = kCLOCK_NPU_ClockRoot_MAINPLL_DIVOUT1;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_NPU_ROOTCLK, &rootCfg);

    /* MEDIABUS_ROOTCLK: source SYSPLL_DIVOUT2 (500 MHz) / 2 = 250 MHz. */
    rootCfg.mux = kCLOCK_MEDIABUS_ClockRoot_SYSPLL_DIVOUT2;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_MEDIABUS_ROOTCLK, &rootCfg);
}

/*
 * CMPT domain clock roots (5 slices on CMPT_CCM):
 *
 *  Slice  Root Name       Mux  Source   Div  Yield
 *  -----  -------------  ---  ------  ---  ----------
 *     0   cmpt_clk         0  MAIN      1  1000 MHz
 *     1   cpu_clk          0  CPU       1  1000 MHz
 *     2   npu_clk          0  NPU       1  792 MHz
 *     3   systick_clk0     1  SXOSC     1  24 MHz
 *     4   systick_clk1     1  SXOSC     1  24 MHz
 */
static void ConfigCGUDig_CMPT(void)
{
    clock_root_config_t rootCfg = {0};
    rootCfg.sndDiv              = 1U;

    rootCfg.mux = kCLOCK_CMPT_ClockRoot_MAIN;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CMPT_cmpt_clk, &rootCfg);

    rootCfg.mux = kCLOCK_CPU_ClockRoot_CPU;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CMPT_cpu_clk, &rootCfg);

    rootCfg.mux = kCLOCK_NPU_ClockRoot_NPU;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CMPT_npu_clk, &rootCfg);

    rootCfg.mux = kCLOCK_SYSTICK0_ClockRoot_SXOSC;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CMPT_systick_clk0, &rootCfg);

    rootCfg.mux = kCLOCK_SYSTICK1_ClockRoot_SXOSC;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_CMPT_systick_clk1, &rootCfg);
}

/*
 * MAIN domain clock roots (34 slices on MAIN_CCM):
 *
 *  Slice  Root Name       Mux  Source    Div  Yield
 *  -----  -------------  ---  -------  ---  ----------
 *     3   i3c0_fclk        0  PERI3     10  40 MHz
 *     4   lpi2c0_fclk      0  PERI3     10  40 MHz
 *     5   lpi2c1_fclk      0  PERI3     10  40 MHz
 *     6   lpspi0_fclk      0  PERI2      3  133.33 MHz
 *     7   lpspi1_fclk      0  PERI2      3  133.33 MHz
 *     8   lpspi2_fclk      0  PERI2      3  133.33 MHz
 *     9   lpspi3_fclk      0  PERI2      3  133.33 MHz
 *    10   lpspi4_fclk      0  PERI2      3  133.33 MHz
 *    11   lpuart0_fclk     0  PERI3      5  80 MHz
 *    12   lpuart1_fclk     0  PERI3      5  80 MHz
 *    13   lpuart2_fclk     0  PERI3      5  80 MHz
 *    14   lpuart3_fclk     0  PERI3      5  80 MHz
 *    15   lpuart4_fclk     0  PERI3      5  80 MHz
 *    16   lpuart5_fclk     0  PERI3      5  80 MHz
 *    17   flexcan0_fclk    1  PERI4      5  80 MHz
 *    18   flexcan1_fclk    1  PERI4      5  80 MHz
 *    19   flexcan2_fclk    1  PERI4      5  80 MHz
 *    20   flexcan_gfclk    3  SXOSC      1  24 MHz
 *    21   qtpm0_fclk       0  PERI3      2  200 MHz
 *    22   lpit0_fclk       0  PERI3     10  40 MHz
 *    23   lpit1_fclk       0  PERI3     10  40 MHz
 *    24   adc0_fclk        0  PERI3      4  100 MHz
 *    25   adc1_fclk        0  PERI3      4  100 MHz
 *    26   sinc0_fclk       2  FRO192M    1  192 MHz
 *    27   sinc1_fclk       2  FRO192M    1  192 MHz
 *    28   flexio0_fclk     1  PERI5      2  200 MHz
 *    29   flexio1_fclk     1  PERI5      2  200 MHz
 *    30   flexio2_fclk     1  PERI5      2  200 MHz
 *    31   tpiu_clk         0  PERI3      4  100 MHz
 *    32   cssi_refclk      0  FRO96M     1  96 MHz
 *    33   otp_clk          0  SXOSC      1  24 MHz
 *    34   clkout           0  PERI3      1  400 MHz
 *    35   fro192m          0  FRO192M    1  192 MHz
 *    36   ulp32k           0  ULP32K     1  32 kHz
 */
static void ConfigCGUDig_MAIN(void)
{
    clock_root_config_t rootCfg = {0};
    rootCfg.sndDiv = 1U;

    rootCfg.mux = kCLOCK_I3C0_ClockRoot_PERI3;
    rootCfg.div = 10U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_i3c0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPI2C0_ClockRoot_PERI3;
    rootCfg.div = 10U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpi2c0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPI2C1_ClockRoot_PERI3;
    rootCfg.div = 10U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpi2c1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPSPI0_ClockRoot_PERI2;
    rootCfg.div = 3U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpspi0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPSPI1_ClockRoot_PERI2;
    rootCfg.div = 3U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpspi1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPSPI2_ClockRoot_PERI2;
    rootCfg.div = 3U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpspi2_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPSPI3_ClockRoot_PERI2;
    rootCfg.div = 3U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpspi3_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPSPI4_ClockRoot_PERI2;
    rootCfg.div = 3U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpspi4_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPUART0_ClockRoot_PERI3;
    rootCfg.div = 5U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpuart0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPUART1_ClockRoot_PERI3;
    rootCfg.div = 5U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpuart1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPUART2_ClockRoot_PERI3;
    rootCfg.div = 5U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpuart2_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPUART3_ClockRoot_PERI3;
    rootCfg.div = 5U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpuart3_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPUART4_ClockRoot_PERI3;
    rootCfg.div = 5U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpuart4_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPUART5_ClockRoot_PERI3;
    rootCfg.div = 5U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpuart5_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_FLEXCAN0_ClockRoot_PERI4;
    rootCfg.div = 5U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_flexcan0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_FLEXCAN1_ClockRoot_PERI4;
    rootCfg.div = 5U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_flexcan1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_FLEXCAN2_ClockRoot_PERI4;
    rootCfg.div = 5U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_flexcan2_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_FLEXCAN_GFCLK_ClockRoot_SXOSC;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_flexcan_gfclk, &rootCfg);

    rootCfg.mux = kCLOCK_QTPM0_ClockRoot_PERI3;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_qtpm0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPIT0_ClockRoot_PERI3;
    rootCfg.div = 10U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpit0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPIT1_ClockRoot_PERI3;
    rootCfg.div = 10U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_lpit1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_ADC0_ClockRoot_PERI3;
    rootCfg.div = 4U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_adc0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_ADC1_ClockRoot_PERI3;
    rootCfg.div = 4U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_adc1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_SINC0_ClockRoot_FRO192M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_sinc0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_SINC1_ClockRoot_FRO192M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_sinc1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_FLEXIO0_ClockRoot_PERI5;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_flexio0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_FLEXIO1_ClockRoot_PERI5;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_flexio1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_FLEXIO2_ClockRoot_PERI5;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_flexio2_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_TPIU_ClockRoot_PERI3;
    rootCfg.div = 4U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_tpiu_clk, &rootCfg);

    rootCfg.mux = kCLOCK_CSSI_REFCLK_ClockRoot_FRO96M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_cssi_refclk, &rootCfg);

    rootCfg.mux = kCLOCK_OTP_ClockRoot_SXOSC;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_otp_clk, &rootCfg);

    rootCfg.mux = kCLOCK_MAIN_CLKOUT_ClockRoot_PERI3;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_clkout, &rootCfg);

    rootCfg.mux = kCLOCK_MAIN_MAIN_FRO192M_ClockRoot_FRO192M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_fro192m, &rootCfg);

    rootCfg.mux = kCLOCK_MAIN_MAIN_ULP32K_ClockRoot_ULP32K;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_MAIN_ulp32k, &rootCfg);
}

/*
 * MEDIA domain clock roots (10 slices on MEDIA_CCM):
 *
 *  Slice  Root Name                Mux  Source         Div  Yield
 *  -----  ----------------------  ---  ------------  ---  ----------
 *     0   media_clk                 0  MEDIABUS        1  333.33 MHz
 *     1   mediapll_clk              0  MAINPLLDIV10    1  200 MHz
 *     2   mipicsi_escclk            0  PERI5           2  200 MHz
 *     3   mipicsi_clk               0  PERI5           2  200 MHz
 *     4   mipidsi_escclk_divided    0  PERI5           2  200 MHz
 *     5   mipidsi_refclk            0  SXOSC           1  24 MHz
 *     6   mipidsi_clk               0  PERI5           2  200 MHz
 *     7   reformat_fclk             0  PERI5           2  200 MHz
 *     8   dcpixel_fclk              0  PERI5           2  200 MHz
 *     9   csi_mclkout               0  PERI5           2  200 MHz
 */
static void ConfigCGUDig_MEDIA(void)
{
    POWER_SetDomainRunMode(kPOWER_DomainMedia, kPDCON_EventNoneOrActive);
    clock_root_config_t rootCfg = {0};
    rootCfg.sndDiv              = 1U;

    rootCfg.mux = kCLOCK_MEDIA_ClockRoot_MEDIABUS;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_media_clk, &rootCfg);

    rootCfg.mux = kCLOCK_MEDIAPLL_ClockRoot_MAINPLLDIV10;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_mediapll_clk, &rootCfg);

    rootCfg.mux = kCLOCK_MIPICSI_ESCCLK_ClockRoot_PERI5;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_mipicsi_escclk, &rootCfg);

    rootCfg.mux = kCLOCK_MIPICSI_ClockRoot_PERI5;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_mipicsi_clk, &rootCfg);

    rootCfg.mux = kCLOCK_MIPIDSI_ESCCLK_ClockRoot_PERI5;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_mipidsi_escclk_divided, &rootCfg);

    rootCfg.mux = kCLOCK_MIPIDSI_REFCLK_ClockRoot_SXOSC;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_mipidsi_refclk, &rootCfg);

    rootCfg.mux = kCLOCK_MIPIDSI_ClockRoot_PERI5;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_mipidsi_clk, &rootCfg);

    rootCfg.mux = kCLOCK_REFORMAT_ClockRoot_PERI5;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_reformat_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_DCPIXEL_ClockRoot_PERI5;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_dcpixel_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_CSI_MCLKOUT_ClockRoot_PERI5;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_csi_mclkout, &rootCfg);
}

/*
 * AUDIO domain clock roots (11 slices on AUDIO_CCM):
 *
 *  Slice  Root Name      Mux  Source     Div  Yield
 *  -----  ------------  ---  --------  ---  -------
 *     0   audio_clk       0  AUDIOBUS    1  200 MHz
 *     1   dmic0_appclk    0  PERI6       1  24 MHz
 *     2   sai0_mclk0      0  PERI6       1  24 MHz
 *     3   sai0_mclk1      0  PERI6       1  24 MHz
 *     4   sai1_mclk0      0  PERI6       1  24 MHz
 *     5   sai1_mclk1      0  PERI6       1  24 MHz
 *     6   sai2_mclk0      0  PERI6       1  24 MHz
 *     7   sai2_mclk1      0  PERI6       1  24 MHz
 *     8   spdif_txclk     0  PERI6       1  24 MHz
 *     9   spdif_cdrclk    0  PERI6       1  24 MHz
 *    10   asrc_clk        0  PERI6       1  24 MHz
 */
static void ConfigCGUDig_AUDIO(void)
{
    clock_root_config_t rootCfg = {0};
    rootCfg.sndDiv              = 1U;

    rootCfg.mux = kCLOCK_AUDIO_CLK_ClockRoot_AUDIOBUS;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_audio_clk, &rootCfg);

    rootCfg.mux = kCLOCK_DMIC0_APPCLK_ClockRoot_PERI6;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_dmic0_appclk, &rootCfg);

    rootCfg.mux = kCLOCK_SAI0_MCLK0_ClockRoot_PERI6;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_sai0_mclk0, &rootCfg);

    rootCfg.mux = kCLOCK_SAI0_MCLK1_ClockRoot_PERI6;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_sai0_mclk1, &rootCfg);

    rootCfg.mux = kCLOCK_SAI1_MCLK0_ClockRoot_PERI6;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_sai1_mclk0, &rootCfg);

    rootCfg.mux = kCLOCK_SAI1_MCLK1_ClockRoot_PERI6;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_sai1_mclk1, &rootCfg);

    rootCfg.mux = kCLOCK_SAI2_MCLK0_ClockRoot_PERI6;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_sai2_mclk0, &rootCfg);

    rootCfg.mux = kCLOCK_SAI2_MCLK1_ClockRoot_PERI6;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_sai2_mclk1, &rootCfg);

    rootCfg.mux = kCLOCK_SPDIF_TXCLK_ClockRoot_PERI6;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_spdif_txclk, &rootCfg);

    rootCfg.mux = kCLOCK_SPDIF_CDRCLK_ClockRoot_PERI6;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_spdif_cdrclk, &rootCfg);

    rootCfg.mux = kCLOCK_ASRC_ClockRoot_PERI6;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_AUDIO_asrc_clk, &rootCfg);
}

/*
 * COMM domain clock roots (17 slices on COMM_CCM):
 *
 *  Slice  Root Name       Mux  Source          Div  Yield
 *  -----  -------------  ---  -------------  ---  -------
 *     0   comm_clk         0  COMMBUS          1  200 MHz
 *     1   comm_ulp32k      0  ULP32K           1  32 kHz
 *     2   usdhc0_fclk      1  COMMPFDX_DIV2    4  99 MHz
 *     3   usdhc1_fclk      1  COMMPFDX_DIV2    4  99 MHz
 *     4   xspir_rootclk    2  SYSPLLDIV4       2  250 MHz
 *     5   usb0_phyclk      0  SXOSC            1  24 MHz
 *     6   usb0_fro48m      0  FRO48M           1  48 MHz
 *     7   usb1_fclk        0  USB1             1  48 MHz
 *     8   usb0_wakeclk     0  ULP32K           1  32 kHz
 *     9   eth0_trxclk      3  MAINPLLDIV8      1  250 MHz
 *    10   eth0_timerclk    0  COMMBUS          1  200 MHz
 *    11   eth1_trxclk      3  MAINPLLDIV8      1  250 MHz
 *    12   eth1_timerclk    0  COMMBUS          1  200 MHz
 *    13   eth_refclk       0  COMMBUS          4  50 MHz
 *    14   xeno0_liwclk     0  COMMBUS          2  100 MHz
 *    15   xeno1_liwclk     0  COMMBUS          2  100 MHz
 *    16   dll_refclk       0  MAINPLLDIV10     1  200 MHz
 */
static void ConfigCGUDig_COMM(void)
{
    clock_root_config_t rootCfg = {0};
    rootCfg.sndDiv              = 1U;

    rootCfg.mux = kCLOCK_COMM_ClockRoot_COMMBUS;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_comm_clk, &rootCfg);

    rootCfg.mux = kCLOCK_COMM_ULP32K_ClockRoot_ULP32K;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_comm_ulp32k, &rootCfg);

    rootCfg.mux = kCLOCK_USDHC0_ClockRoot_COMMPFDX_DIV2;
    rootCfg.div = 4U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_usdhc0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_USDHC1_ClockRoot_COMMPFDX_DIV2;
    rootCfg.div = 4U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_usdhc1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_XSPIR_ClockRoot_SYSPLLDIV4;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_xspir_rootclk, &rootCfg);

    rootCfg.mux = kCLOCK_USB0_PHYCLK_ClockRoot_SXOSC;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_usb0_phyclk, &rootCfg);

    rootCfg.mux = kCLOCK_USB0_FRO48M_ClockRoot_FRO48M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_usb0_fro48m, &rootCfg);

    rootCfg.mux = kCLOCK_USB1_ClockRoot_USB1;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_usb1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_USB0_WAKECLK_ClockRoot_ULP32K;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_usb0_wakeclk, &rootCfg);

    rootCfg.mux = kCLOCK_ETH0_TRXCLK_ClockRoot_MAINPLLDIV8;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_eth0_trxclk, &rootCfg);

    rootCfg.mux = kCLOCK_ETH0_TIMERCLK_ClockRoot_COMMBUS;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_eth0_timerclk, &rootCfg);

    rootCfg.mux = kCLOCK_ETH1_TRXCLK_ClockRoot_MAINPLLDIV8;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_eth1_trxclk, &rootCfg);

    rootCfg.mux = kCLOCK_ETH1_TIMERCLK_ClockRoot_COMMBUS;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_eth1_timerclk, &rootCfg);

    rootCfg.mux = kCLOCK_ETH_REFCLK_ClockRoot_COMMBUS;
    rootCfg.div = 4U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_eth_refclk, &rootCfg);

    rootCfg.mux = kCLOCK_XENO0_LIWCLK_ClockRoot_COMMBUS;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_xeno0_liwclk, &rootCfg);

    rootCfg.mux = kCLOCK_XENO1_LIWCLK_ClockRoot_COMMBUS;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_xeno1_liwclk, &rootCfg);

    rootCfg.mux = kCLOCK_DLL_REFCLK_ClockRoot_MAINPLLDIV10;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_dll_refclk, &rootCfg);
}

/*
 * WAKE domain clock roots (27 slices on WAKE_CCM):
 *
 *  Slice  Root Name      Mux  Source       Div  Yield
 *  -----  ------------  ---  ----------  ---  ------
 *     0   wake_clk        0  WAKEBUS       1  40 MHz
 *     1   wake_sxosc      0  SXOSC         1  24 MHz
 *     2   wake_lp1m       0  LP1M_WAKE     1  1 MHz
 *     3   wake_lp12m      0  LP12M_WAKE    1  12 MHz
 *     4   wake_ulp32k     0  ULP32K        1  32 kHz
 *     5   wake_lpclk      0  LP1M_WAKE     1  1 MHz
 *     6   i3c0_fclk       0  PERI7         1  38.4 MHz
 *     7   lpi2c0_fclk     0  PERI7         1  38.4 MHz
 *     8   lpi2c1_fclk     0  PERI7         1  38.4 MHz
 *     9   lpspi0_fclk     0  PERI7         1  38.4 MHz
 *    10   lpuart0_fclk    0  PERI7         1  38.4 MHz
 *    11   lpuart1_fclk    0  PERI7         1  38.4 MHz
 *    12   dmic1_appclk    0  PERI7         1  38.4 MHz
 *    13   qtpm0_fclk      0  PERI7         1  38.4 MHz
 *    14   lptmr0_fclk     1  LP1M_WAKE     1  1 MHz
 *    15   lptmr1_fclk     1  LP1M_WAKE     1  1 MHz
 *    16   swt0_fclk       1  LP1M_WAKE     1  1 MHz
 *    17   swt1_fclk       1  LP1M_WAKE     1  1 MHz
 *    18   ewm_fclk        1  LP1M_WAKE     1  1 MHz
 *    19   acmp0_fclk      0  PERI7         1  38.4 MHz
 *    20   acmp1_fclk      0  PERI7         1  38.4 MHz
 *    21   acmp2_fclk      0  PERI7         1  38.4 MHz
 *    22   acmp3_fclk      0  PERI7         1  38.4 MHz
 *    23   acmp0_rrclk     1  FRO24M        1  24 MHz
 *    24   acmp1_rrclk     1  FRO24M        1  24 MHz
 *    25   acmp2_rrclk     1  FRO24M        1  24 MHz
 *    26   acmp3_rrclk     1  FRO24M        1  24 MHz
 */
/* WAKE_SS domain -- always-on peripheral roots.
 *
 * TBD (silicon / RM): several roots in this helper select a LP1M_WAKE,
 * LP12M_WAKE, or LP2M_WAKE source via mux. The corresponding CGU SS slice
 * entries (LP1M_WAKE_ROOTCLK, LP12M_WAKE_ROOTCLK, LP2M_WAKE_ROOTCLK in the
 * CLK_SLICES list) are marked `#feedthrough` -- i.e. silicon
 * implements the WAKE-domain LP signal but does NOT expose a software-
 * configurable slice for it. Per RM 117.4.7 the FRO_12M block outputs
 * CLK_FRO12M_WAKE (level-shifter version under VDD_0V8) and CLK_FRO1M
 * (12 MHz / 12), which we assume map to LP12M_WAKE and LP1M_WAKE
 * respectively. LP2M_WAKE has no obvious counterpart in RM 117.4.7 and
 * may be sourced from a separate always-on 2 MHz oscillator outside the
 * CGUANA scope (likely under VBAT/PMU).
 *
 * Please confirm with the IC team / RM clock tree diagram that:
 *   1. LP1M_WAKE, LP12M_WAKE, LP2M_WAKE signals are actually routed to
 *      the WAKE_CCM domain and live at boot time.
 *   2. The peripherals that select these (WAKE_LPTMR0/1, WAKE_SWT0/1,
 *      WAKE_EWM, WAKE_LPCLK) receive a usable clock with mux=1.
 *   3. If any of LP1M_WAKE / LP12M_WAKE / LP2M_WAKE need separate
 *      software bring-up (e.g. a PMU register), it must happen before
 *      BOARD_BootClockRUN()'s subsystem stage -- add that init somewhere
 *      in ConfigCGUAna() or in board.c BOARD_InitHardware(). */
static void ConfigCGUDig_WAKE(void)
{
    clock_root_config_t rootCfg = {0};
    rootCfg.sndDiv              = 1U;

    rootCfg.mux = kCLOCK_WAKE_ClockRoot_WAKEBUS;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_wake_clk, &rootCfg);

    rootCfg.mux = kCLOCK_WAKE_SXOSC_ClockRoot_SXOSC;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_wake_sxosc, &rootCfg);

    rootCfg.mux = kCLOCK_WAKE_LP1M_ClockRoot_LP1M_WAKE;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_wake_lp1m, &rootCfg);

    rootCfg.mux = kCLOCK_WAKE_LP12M_ClockRoot_LP12M_WAKE;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_wake_lp12m, &rootCfg);

    rootCfg.mux = kCLOCK_WAKE_ULP32K_ClockRoot_ULP32K;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_wake_ulp32k, &rootCfg);

    rootCfg.mux = kCLOCK_WAKE_LPCLK_ClockRoot_LP1M_WAKE;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_wake_lpclk, &rootCfg);

    rootCfg.mux = kCLOCK_WAKE_I3C0_ClockRoot_PERI7;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_i3c0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_WAKE_LPI2C0_ClockRoot_PERI7;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_lpi2c0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_WAKE_LPI2C1_ClockRoot_PERI7;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_lpi2c1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_WAKE_LPSPI0_ClockRoot_PERI7;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_lpspi0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_WAKE_LPUART0_ClockRoot_PERI7;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_lpuart0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_WAKE_LPUART1_ClockRoot_PERI7;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_lpuart1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_DMIC1_APPCLK_ClockRoot_PERI7;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_dmic1_appclk, &rootCfg);

    rootCfg.mux = kCLOCK_QTPM0_ClockRoot_PERI7;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_qtpm0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPTMR0_ClockRoot_LP1M_WAKE;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_lptmr0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_LPTMR1_ClockRoot_LP1M_WAKE;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_lptmr1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_SWT0_ClockRoot_LP1M_WAKE;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_swt0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_SWT1_ClockRoot_LP1M_WAKE;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_swt1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_EWM_ClockRoot_LP1M_WAKE;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_ewm_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_ACMP0_ClockRoot_PERI7;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_acmp0_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_ACMP1_ClockRoot_PERI7;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_acmp1_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_ACMP2_ClockRoot_PERI7;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_acmp2_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_ACMP3_ClockRoot_PERI7;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_acmp3_fclk, &rootCfg);

    rootCfg.mux = kCLOCK_ACMP0_RRCLK_ClockRoot_FRO24M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_acmp0_rrclk, &rootCfg);

    rootCfg.mux = kCLOCK_ACMP1_RRCLK_ClockRoot_FRO24M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_acmp1_rrclk, &rootCfg);

    rootCfg.mux = kCLOCK_ACMP2_RRCLK_ClockRoot_FRO24M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_acmp2_rrclk, &rootCfg);

    rootCfg.mux = kCLOCK_ACMP3_RRCLK_ClockRoot_FRO24M;
    rootCfg.div = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_WAKE_acmp3_rrclk, &rootCfg);
}
