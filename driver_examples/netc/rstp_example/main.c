/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "app.h"
#include "netc_switch.h"
#include "stp_freertos_adapter.h"

int main(void)
{
    status_t result                 = kStatus_Success;
    uint8_t bridge_mac[6]           = {0};

    /* Init board hardware. */
    BOARD_InitHardware();

    result = APP_MDIO_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nMDIO Init failed!\r\n");
        return result;
    }

    result = APP_PHY_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nPHY Init failed!\r\n");
        return result;
    }

    result = APP_SWT_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nSwitch Init failed!\r\n");
        return result;
    }

    APP_GetBridgeMACAddress(bridge_mac);

    stp_freertos_init(bridge_mac, EXAMPLE_SWT_MAX_PORT_NUM); /* one STP port per switch front port */

    stp_task_create();

    stp_freertos_start();

    netc_rx_task_create();
    phy_poll_task_create();

    vTaskStartScheduler();

    while (1)
    {
    }
}
