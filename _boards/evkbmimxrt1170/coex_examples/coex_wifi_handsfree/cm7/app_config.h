/*
 *  Copyright 2021-2026 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#if defined(__MCUXPRESSO)
#include "app_mcuxpresso_config.h"
#endif

/* If OT or BLE is enabled, the vApplicationHook defined by the app should be used
 * instead of the private definition of WIFI, use CONFIG_COEX_APP macro to select.
 */
#if (CONFIG_OT_CLI || (!CONFIG_DISABLE_BLE))
#define CONFIG_COEX_APP                 1
#else
#define CONFIG_COEX_APP                 0
#endif

/* OSA task: requires PRIORITY_RTOS_TO_OSA conversion */
#define SERIAL_MANAGER_TASK_PRIORITY (PRIORITY_RTOS_TO_OSA((CONFIG_NUM_PREEMPT_PRIORITIES - 3)))
/* edgefast_open k_thread_create task: uses FreeRTOS priority directly via K_PRIO_COOP(), no OSA conversion */
#define SHELL_TASK_PRIORITY          (CONFIG_NUM_PREEMPT_PRIORITIES - 3)

/* HFP SCO audio: TX clock sync mode for the module-side PCM path (used by the
 * board hardware_init.c SAI config, same as the stock handsfree example). */
#define FLASH_ADAPTER_SIZE          0x10000
#define PCM_MODE_CONFIG_TX_CLK_SYNC 1

#include "edgefast_open_config.h"
#include "wifi_bt_module_config.h"
#include "wifi_config.h"

#endif /* APP_CONFIG_H */
