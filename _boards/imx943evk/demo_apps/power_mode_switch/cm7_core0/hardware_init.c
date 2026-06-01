/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "rsc_table.h"
#include "app_srtm.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    /* copy resource table to destination address(TCM) */
    copyResourceTable();

    APP_SRTM_Init();
    APP_SRTM_StartCommunication();
}
