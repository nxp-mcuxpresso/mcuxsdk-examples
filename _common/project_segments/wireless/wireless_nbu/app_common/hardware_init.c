/*!
 * Copyright 2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file hardware_init.c
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */
#include "app.h"
#include "fwk_platform.h"

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */
void BOARD_InitHardware(void)
{
    /* Configure FRO192M clock */
#if !defined(FPGA_TARGET) || (FPGA_TARGET == 0)
    PLATFORM_InitFro192M();
#endif
}
