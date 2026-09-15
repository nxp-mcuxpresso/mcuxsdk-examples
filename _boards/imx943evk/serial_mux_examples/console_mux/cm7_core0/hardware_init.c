/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Virtual console client: no UART is owned here, only system/clock
     * bring-up. The console travels through the shared-memory slots polled by
     * the hub (cm33_core1) and out on the hub mux UART as channel 1. */
    SystemPlatformInit();
    BOARD_BootClockRUN();
    BOARD_ConfigMPU();
}
/*${function:end}*/
