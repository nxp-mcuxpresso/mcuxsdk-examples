/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_debug_console.h"

void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitBUTTONsPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
}
