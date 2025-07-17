/*
 * Copyright 2021-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board_platform.h"
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fwk_platform.h"
#include "fwk_debug.h"

#ifndef gBoardUseDcdc_d
#define gBoardUseDcdc_d 1
#endif

#if defined(gBoardUseDcdc_d) && (gBoardUseDcdc_d > 0)
#include "board_dcdc.h"
#endif

#if !defined(FPGA_TARGET) || (FPGA_TARGET == 0)
#if defined(gBoardUseFro32k_d) && (gBoardUseFro32k_d > 0)
#include "fsl_ccm32k.h"
#endif
#endif

#if defined(BOARD_DBG_SWO_PIN_ENABLE) && (BOARD_DBG_SWO_PIN_ENABLE != 0)
#include "board_debug.h"
#endif

#if defined(BOARD_DBG_SWO_CORE_FUNNEL) && (BOARD_DBG_SWO_CORE_FUNNEL != 0)
#include "fwk_debug_swo.h"
#endif

#if defined(DBG_SWO_PIN_ENABLE) && (DBG_SWO_PIN_ENABLE != 0)
#include "fwk_debug_swo.h"
#endif

#if defined(BOARD_DBG_NBU_ENABLE) && (BOARD_DBG_NBU_ENABLE != 0)
#include "board_debug_nbu.h"
#endif

/*${header:end}*/

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */
#if defined(gBoardUseXtal32MTempComp) && (gBoardUseXtal32MTempComp > 0)
/*!
 * \brief This structure defines a LUT for the XCVR 32MHz crystal trimming
 * compensation depending on the temperature for a range of [-39C,127C]
 * Those are default values provided by NXP but can be customized depending on
 * hardware. Those values are provided as a mean across several samples.
 * gBoardUseXtal32MTempComp flag must be defined in board_platform.h to enable this
 * feature.
 * IMPORTANT: This LUT applies only to KW47/MCXW72 platforms, using it on other
 * platforms could cause unexpected results.
 *
 */
const xtal_temp_comp_lut_t xtal32MTempCompLut = {
    .min_temp_degc = -39,
    .max_temp_degc = 130,
    .trim_below_min_temp = 2U,
    .trim_above_max_temp = 50U,
    .temp_step_degc = 10,
    .xtal_trim_lut = {4U, 7U, 9U, 9U, 9U, 7U, 5U, 4U, 3U, 2U, 2U, 2U, 3U, 6U, 11U, 23U, 43U},
};
#endif /* defined(gBoardUseXtal32MTempComp) && (gBoardUseXtal32MTempComp > 0) */

/*${function:start}*/

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

#if !defined(FPGA_TARGET) || (FPGA_TARGET == 0)
static void BOARD_InitOsc32MHzConfig(void);
#endif
/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void BOARD_InitHardware(void)
{
    BOARD_DBGCONFIGINIT(TRUE); // internal debug

#if !defined(FPGA_TARGET) || (FPGA_TARGET == 0)
    /* Set default value before XTAL start up but can be updated at run time through
       PLATFORM_Update32MhzTrimFromHwParam() from value stored in flash in hw param*/
    BOARD_InitOsc32MHzConfig();

#if defined(gAppHighSystemClockFrequency_d) && (gAppHighSystemClockFrequency_d > 0)
    /* Set Core frequency to 96Mhz , core voltage to 1.1v */
    BOARD_BootClockHSRUN();
#else
    /* Set Core frequency to 48Mhz , core voltage to 1.0v */
    BOARD_BootClockRUN();
#endif

#if defined(gBoardUseFro32k_d) && (gBoardUseFro32k_d > 0)
    /* Enable and select the fro32k as 32k clock source and disable osc32k
     * The NBU firmware embeds a module capable of trimming the fro32k to get it as close as possible from 32768Hz
     * frequency
     * */
    (void)PLATFORM_InitFro32K();
#else
    /* Initialize and start osc32k
     * Note: osc32k is not yet selected as 32k clock source, the switch is done by the radio when the oscillator is
     * ready. For this mechanism to work, the osc32k must be enabled before starting the radio core, then the radio core
     * will detect the osc32k is enabled and switch to it when it is ready.
     * The switch can still be done by the host processor by calling PLATFORM_SwitchToOsc32k if needed.
     * The fro32k MUST NOT be disabled now, this will be handled by the radio core or in PLATFORM_SwitchToOsc32k. */
    (void)PLATFORM_InitOsc32K();
#endif
#endif /* #if !defined(FPGA_TARGET) || (FPGA_TARGET == 0) */

    CLOCK_EnableClock(kCLOCK_Tstmr0);

#if defined(gBoardUseDcdc_d) && (gBoardUseDcdc_d > 0)
    /* Initialize DCDC and apply optimized configuration */
    BOARD_InitDcdc();
#endif /* #if defined(gBoardUseDcdc_d) && (gBoardUseDcdc_d > 0) */

    /* Configure any pins by default during device Initialization.
     * Shall be called first. Pin configuration can be overriden by any other specific pin API functions below.
     */
    BOARD_InitPins();

#if defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1)
    BOARD_InitDebugConsole();
#endif

#if defined(BOARD_DBG_SWO_PIN_ENABLE) && (BOARD_DBG_SWO_PIN_ENABLE != 0)
    BOARD_InitSWO();
#endif

#if defined(BOARD_DBG_NBU_ENABLE) && (BOARD_DBG_NBU_ENABLE != 0)
    BOARD_DbgNbuInit();
#endif

#if defined(BOARD_DBG_SWO_CORE_FUNNEL) && (BOARD_DBG_SWO_CORE_FUNNEL != 0)
    /* Configure SoC so that SWO can be controlled */
    DBG_InitSWO(BOARD_DBG_SWO_CORE_FUNNEL);
#endif

#if defined(DBG_SWO_PIN_ENABLE) && (DBG_SWO_PIN_ENABLE != 0)
    /* Configure SWO signal routing on GPIO */
    BOARD_InitPinSWO();
    /* Configure SoC so that SWO can be controlled */
    DBG_InitSWOFunnelMuxing(DBG_SWO_FUNNEL_MUXING);
#endif

#if defined(gBoardUseXtal32MTempComp) && (gBoardUseXtal32MTempComp > 0)
    /* Register the XTAL32M temperature compensation LUT
     * When registered, this LUT will be used to calibrate XTAL32M against temperature
     * when a temperature change is detected */
    PLATFORM_RegisterXtal32MTempCompLut(&xtal32MTempCompLut);
#endif
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

#if !defined(FPGA_TARGET) || (FPGA_TARGET == 0)
static void BOARD_InitOsc32MHzConfig(void)
{
    uint32_t rfmc_xo;
    /* Apply a trim value to the crystal oscillator */
    rfmc_xo = RFMC->XO_TEST;

#if defined(BOARD_32MHZ_XTAL_CDAC_VALUE)
    rfmc_xo &= ~(RFMC_XO_TEST_CDAC_MASK);
    rfmc_xo |= RFMC_XO_TEST_CDAC(BOARD_32MHZ_XTAL_CDAC_VALUE);
#endif

#if defined(BOARD_32MHZ_XTAL_ISEL_VALUE)
    rfmc_xo &= ~(RFMC_XO_TEST_ISEL_MASK);
    rfmc_xo |= RFMC_XO_TEST_ISEL(BOARD_32MHZ_XTAL_ISEL_VALUE);
#endif

    RFMC->XO_TEST = rfmc_xo;
    return;
} /* #if !defined(FPGA_TARGET) || (FPGA_TARGET == 0) */
/*${function:end}*/
#endif