/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_iomuxc.h"
#include "fsl_lpuart.h"
#include "fsl_lptmr.h"
#include "power_mode_switch.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "lpm.h"
#include "fsl_mu.h"
#include "rsc_table.h"
#include "app_srtm.h"
#include "sm_platform.h"

/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    /* copy resource table to destination address(TCM) */
    copyResourceTable();
    
    APP_SRTM_Init();
    APP_SRTM_StartCommunication();
}
