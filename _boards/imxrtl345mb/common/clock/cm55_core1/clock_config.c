/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if (IMXRTL3459AAA_cm55_core1_SERIES)
#include "fsl_clock.h"
#include "clock_config.h"

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
    /* CM55 core1 (NSS) does not own the CGU; the CSS host (core0) configures
       clocks before releasing core1, so there is nothing to do here. */
}

#endif
