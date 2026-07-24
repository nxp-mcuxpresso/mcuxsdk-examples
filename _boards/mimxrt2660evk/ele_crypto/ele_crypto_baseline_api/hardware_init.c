/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();

    /* TODO(RT2660): ELE/Sentinel clock retune is not performed here. The
     * common example body contains a clock-change block gated by
     * FSL_FEATURE_ELE_S4XX which is omitted on RT2660. When the Sentinel
     * clock model is finalised for RT2660 (fsl_clock.h enums + sndDiv
     * semantics), add an equivalent retune here or in the gated block. */
}
/*${function:end}*/
