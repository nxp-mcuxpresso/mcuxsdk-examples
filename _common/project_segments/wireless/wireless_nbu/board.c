/*!
 * Copyright 2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file board.c
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */
#include <stdint.h>

#include "fsl_common.h"
#include "board.h"

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */
#if !defined(FPGA_TARGET) || (FPGA_TARGET == 0)
static const uint32_t froPostDivFreq[] = {
    16000000U, 24000000U, 32000000U, 48000000U, 64000000U
};
#endif

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */
#if !defined(FPGA_TARGET) || (FPGA_TARGET == 0)
uint32_t BOARD_GetSystemCoreClockSel(void)
{
    return (FRO192M0->FROCCSR & FRO192M_FROCCSR_POSTDIV_SEL_MASK) >> FRO192M_FROCCSR_POSTDIV_SEL_SHIFT;
}
#endif

void BOARD_SystemCoreClockUpdate(void)
{
#if !defined(FPGA_TARGET) || (FPGA_TARGET == 0)
    uint32_t froPostDivSel = BOARD_GetSystemCoreClockSel();

    SystemCoreClock = froPostDivFreq[froPostDivSel];
#endif
}

uint32_t BOARD_GetSystemCoreClockFreq(void)
{
#if !defined(FPGA_TARGET) || (FPGA_TARGET == 0)
    BOARD_SystemCoreClockUpdate();
#endif
    return SystemCoreClock;
}
