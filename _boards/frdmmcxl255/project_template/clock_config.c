/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_clock.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
    BOARD_BootClockFRO96M();
}

void BOARD_BootClockFRO12M(void)
{
    CLOCK_SetupFRO12MClocking();
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);
    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);
    SystemCoreClock = 12000000U;
}

void BOARD_BootClockFRO24M(void)
{
    CLOCK_SetupFROHFClocking(48000000U, 0U);
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 2U);
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);
    SystemCoreClock = 24000000U;
}

void BOARD_BootClockFRO48M(void)
{
    CLOCK_SetupFROHFClocking(48000000U, 0U);
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);
    SystemCoreClock = 48000000U;
}

void BOARD_BootClockFRO64M(void)
{
    CLOCK_SetupFROHFClocking(64000000U, 0U);
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);
    SystemCoreClock = 64000000U;
}

void BOARD_BootClockFRO96M(void)
{
    CLOCK_SetupFROHFClocking(96000000U, 0U);
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);
    SystemCoreClock = 96000000U;
}
