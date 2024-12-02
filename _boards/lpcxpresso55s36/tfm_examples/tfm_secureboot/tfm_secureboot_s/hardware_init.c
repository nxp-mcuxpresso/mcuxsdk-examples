/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "cmsis.h"

#define ITRC_STATUS        *((unsigned int *)0x5000F020)
#define DISABLE_ITR_RESET  *((unsigned int *)0x5000F028)
#define DISABLE_ITR_RESET1 *((unsigned int *)0x5000F02C)
#define DISABLE_GDET_ITR   *((unsigned int *)0x50030040)

void BOARD_InitHardware(void)
{
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();

    DISABLE_ITR_RESET  = 0xAAAAAAAA;
    DISABLE_ITR_RESET1 = 0xAAAAAAAA;
    DISABLE_GDET_ITR   = DISABLE_GDET_ITR & 0xFFFFFFFFD;

    /* Flash operations (erase, blank check, program) and reading single word can only be performed for CPU frequencies
    of up to 100 MHz. Cannot be performed for frequencies above 100 MHz. */
    BOARD_BootClockFROHF96M();

    // DM avoid Hard Fault    BOARD_InitDebugConsole();
}

void SystemInit(void)
{
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));    /* set CP10, CP11 Full Access in Secure mode */
    SCB_NS->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10, CP11 Full Access in Non-secure mode */
#endif                                                    /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

    SCB->CPACR |= ((3UL << 0 * 2) | (3UL << 1 * 2));    /* set CP0, CP1 Full Access in Secure mode (enable PowerQuad) */
    SCB_NS->CPACR |= ((3UL << 0 * 2) | (3UL << 1 * 2)); /* set CP0, CP1 Full Access in Normal mode (enable PowerQuad) */

    SCB->NSACR |= ((3UL << 0) | (3UL << 10));           /* enable CP0, CP1, CP10, CP11 Non-secure Access */

    extern void *__VECTOR_TABLE[];
    SCB->VTOR = (uint32_t) & (__VECTOR_TABLE[0]);

    SYSCON->TRACECLKDIV = 0;
/* Optionally enable RAM banks that may be off by default at reset */
#if !defined(DONT_ENABLE_DISABLED_RAMBANKS)
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL0_SRAM_CTRL1_MASK | SYSCON_AHBCLKCTRL0_SRAM_CTRL2_MASK |
                               SYSCON_AHBCLKCTRL0_SRAM_CTRL3_MASK | SYSCON_AHBCLKCTRL0_SRAM_CTRL4_MASK;
#endif
    /* enable the flash cache LPCAC */
    SYSCON->LPCAC_CTRL &= ~SYSCON_LPCAC_CTRL_DIS_LPCAC_MASK;
}
