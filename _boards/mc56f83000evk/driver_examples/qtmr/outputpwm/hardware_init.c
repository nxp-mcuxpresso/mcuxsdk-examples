/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "peripherals.h"
/*${header:end}*/

/*${function:start}*/
#pragma interrupt alignsp saveall
void ivINT_TMRA_2(void);
void ivINT_TMRA_2(void)
{
    QTMR_IRQ_HANDLER();
}
#pragma interrupt off

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    BOARD_InitDebugConsole();

    SetIRQBasePriority(0);
}
/*${function:end}*/
