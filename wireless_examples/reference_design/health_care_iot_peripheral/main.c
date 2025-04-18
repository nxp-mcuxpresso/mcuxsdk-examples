/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2021-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "safety.h"
#include "fsl_os_abstraction.h"
#include "ble_general.h"
#ifdef ENABLE_LOW_POWER
#include "fsl_pm_core.h"
#endif
#include "hciot_common.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
#ifdef SDK_OS_FREE_RTOS
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    PRINTF("Stack overflow happened in task %s. Application halted.\r\n", pcTaskName);
    while (1)
        ;
}
#endif /* SDK_OS_FREE_RTOS */

void ConnectivityTask(void *argument);

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init OSA: should be called before any other OSA API */
    OSA_Init();

    BOARD_InitHardware();

    HCIOT_InitSupplyMode();

#ifdef ENABLE_LOW_POWER
    PM_Init();
    PM_EnablePowerManager(true);
#endif /* ENABLE_LOW_POWER */

    SAFETY_Init();

    /*won't run here*/
    assert(0);

    return 0;
}
