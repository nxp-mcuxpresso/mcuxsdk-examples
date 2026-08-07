/*
 * Copyright 2020, 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_edma.h"
#include "fsl_trdc.h"
#include "app.h"

/*${header:end}*/

/*${function:start}*/
static void TRDC_EDMA_ResetPermissions(void)
{
}

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitSPIPins();

    BOARD_RequestTRDC();

    TRDC_EDMA_ResetPermissions();
}
/*${function:end}*/
