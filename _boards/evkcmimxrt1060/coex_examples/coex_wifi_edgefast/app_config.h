/*
 *  Copyright 2021-2025 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

/* If OT or BLE is enabled, the vApplicationHook defined by the app should be used
 * instead of the private definition of WIFI, use CONFIG_COEX_APP macro to select.
 */
#if (CONFIG_OT_CLI || (!CONFIG_DISABLE_BLE))
#define CONFIG_COEX_APP                 1
#else
#define CONFIG_COEX_APP                 0
#endif

#define SERIAL_MANAGER_TASK_PRIORITY (PRIORITY_RTOS_TO_OSA((configMAX_PRIORITIES-2)))
#define SHELL_TASK_PRIORITY          (PRIORITY_RTOS_TO_OSA((configMAX_PRIORITIES-2)))

#include "app_bluetooth_config.h"
#include "wifi_config.h"
