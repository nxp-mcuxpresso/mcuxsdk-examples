/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_trdc.h"
/*${header:end}*/

/*${function:start}*/
static void TRDC_EDMA3_EDMA5_ResetPermissions(void)
{

}

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitI2CPins();

    BOARD_RequestTRDC();
    TRDC_EDMA3_EDMA5_ResetPermissions();
}
/*${function:end}*/
