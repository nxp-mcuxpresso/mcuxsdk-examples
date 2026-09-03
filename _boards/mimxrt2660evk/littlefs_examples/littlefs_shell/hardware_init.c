/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    /* The XSPI0 flash pins are muxed by the boot ROM and (on the xspi_nor*
     * targets) the code executes XIP from that flash, so the pins must NOT be
     * re-muxed here: re-applying the mux on the live XIP interface disturbs
     * the running flash for no benefit. */
}

/*${function:end}*/
