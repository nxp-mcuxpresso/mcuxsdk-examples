/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

/*${function:start}*/
void APP_InitDomain(void)
{
}

void APP_DeinitDomain(void)
{
}

uint8_t APP_GetCore0DomainID(void)
{
    return 0U;
}
void APP_InitInterCoreNotifications(void)
{
    /* Init IMU */
    (void)IMU_Init(kIMU_LinkCpu1Cpu2);
}
uint32_t APP_GetInterCoreNotificationsData(void)
{
    uint32_t msg = 0U;
    (void)IMU_ReceiveMsgsBlocking(kIMU_LinkCpu1Cpu2, &msg, 1, NULL);
    return msg;
}
void APP_SetInterCoreNotificationsData(uint32_t data)
{
    (void)IMU_SendMsgsBlocking(kIMU_LinkCpu1Cpu2, &data, 1, false);
}

void APP_BootCore1(void)
{
    /* Release NBU CPU from reset */
    RFMC->RF2P4GHZ_CTRL &= ~(RFMC_RF2P4GHZ_CTRL_CPU_RST_MASK);
    CIU2->CIU2_CPU_CPU2_CTRL = 0x1;

    /* Wait a little to allow Core 1 boot up */
    SDK_DelayAtLeastUs(1000000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
