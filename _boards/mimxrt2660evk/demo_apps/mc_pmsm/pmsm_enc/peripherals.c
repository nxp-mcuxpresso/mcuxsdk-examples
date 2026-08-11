/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "peripherals.h"
#include "fsl_clock.h"
#include "board.h"

void BOARD_InitPeripherals(void)
{
}

void BOARD_InitBootPeripherals(void)
{
    BOARD_InitPeripherals();
}

/* UART clock frequency for FreeMASTER serial transport.
 * Returns the functional clock of LPUART0 (MAIN HSP bus). */
uint32_t BOARD_DebugConsoleSrcFreq(void)
{
    return CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpuart0_fclk);
}
