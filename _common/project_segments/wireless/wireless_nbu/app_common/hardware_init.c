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
#include "fsl_common.h"
#include "app.h"
#include "fwk_platform.h"

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */
void BOARD_InitHardware(void)
{
    /* Configure FRO192M clock */
    PLATFORM_InitFro192M();

    /* Wait for the XTAL to be ready before running anything else
     * The XTAL is started by the main core, so we need to wait for its readiness */
    RF_CMC1->IRQ_CTRL |= RF_CMC1_IRQ_CTRL_RDY_IE_MASK;
    while ((RF_CMC1->IRQ_CTRL & RF_CMC1_IRQ_CTRL_XTAL_RDY_MASK) == 0U)
    {
    }
}
