/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app_srtm.h"
#include "board.h"
#include "pin_mux.h"
#include "rsc_table.h"

extern void app_create_task(void);
extern void app_destroy_task(void);
/*${header:end}*/

/*${function:start}*/
void app_rpmsg_monitor(struct rpmsg_lite_instance *rpmsgHandle, bool ready, void *rpmsgMonitorParam)
{
    if (ready)
    {
        app_create_task();
    }
    else
    {
        app_destroy_task();
    }
}

void BOARD_InitHardware(void)
{
    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    /* copy resource table to destination address(TCM and DRAM) */
    copyResourceTable();

    APP_SRTM_Init();

    /* register callback for restart the app task when A35 reset */
    APP_SRTM_SetRpmsgMonitor(app_rpmsg_monitor, NULL);

    APP_SRTM_StartCommunication();
}
/*${function:end}*/
