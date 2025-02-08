/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_power.h"
#include "fsl_clock.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/** 32KHz oscillator load in cap setting */
#define OSC_CAP_IN_SETTING 5
/** 32KHz oscillator load out cap setting */
#define OSC_CAP_OUT_SETTING 5

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

void BOARD_InitBootClocks(void)
{
    BOARD_BootClockFRO32M();
}

void BOARD_BootClockFRO12M(void)
{
#ifndef SDK_SECONDARY_CORE
    /*!< Set up the clock sources, set up FRO to the 12 MHz */
    CLOCK_SetupFROClocking(kFreq_12MHz);

    /*!< Set FLASH wait states for core */
    CLOCK_SetFLASHAccessCyclesForFreq(kFreq_12MHz);

    /*!< Set AHBCLKDIV divider to value 1 */
    CLOCK_SetClkDiv(kCLOCK_DivAhbClk, 1U, false);

    /*!< Set up clock selectors - Switch MAIN_CLK to FRO12M */
    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);

    /*< Set SystemCoreClock variable. */
    SystemCoreClock = kFreq_12MHz;
#endif
}

void BOARD_BootClockFRO24M(void)
{
    /*!< Switch to FRO 12MHz first to ensure we can change voltage without accidentally
    being below the voltage for current speed */
    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);

    /*!< Set FLASH wait states for core */
    CLOCK_SetFLASHAccessCyclesForFreq(kFreq_24MHz);

    /*!< Set up high frequency FRO output to selected frequency */
    CLOCK_SetupFROClocking(kFreq_24MHz);

    /* Set SystemCoreClock variable. */
    SystemCoreClock = kFreq_24MHz;
}

void BOARD_BootClockFRO32M(void)
{
    /*!< Switch to FRO 12MHz first to ensure we can change voltage without accidentally
    being below the voltage for current speed */
    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);

    /*!< Set FLASH wait states for core */
    CLOCK_SetFLASHAccessCyclesForFreq(kFreq_32MHz);

    /*!< Set up high frequency FRO output to selected frequency */
    CLOCK_SetupFROClocking(kFreq_32MHz);

    /* Set SystemCoreClock variable. */
    SystemCoreClock = kFreq_32MHz;
}

void BOARD_BootClockXTAL32M(void)
{
    /* Set FLASH wait states for core */
    CLOCK_SetFLASHAccessCyclesForFreq(kFreq_32MHz);

    /* Setup external clock (32MHz XTAL) */
    CLOCK_SetupExtClocking(kFreq_32MHz);

    /* Attach external clock to main clock*/
    CLOCK_AttachClk(kEXT_CLK_to_MAIN_CLK);

    /* Set SystemCoreClock variable. */
    SystemCoreClock = kFreq_32MHz;
}

void BOARD_Configure32kOsc(void)
{
    /* Configure 32KHz xtal caps for use with RDM */
    POWER_XTAL32K_ConfigureCaps(OSC_CAP_IN_SETTING, OSC_CAP_OUT_SETTING);
    POWER_PeripheralPowerOn(kPOWERCFG_XTAL32K);
    CLOCK_Select32kOscClkSrc(kCLOCK_Osc32kClockSrc_XTAL);
}
