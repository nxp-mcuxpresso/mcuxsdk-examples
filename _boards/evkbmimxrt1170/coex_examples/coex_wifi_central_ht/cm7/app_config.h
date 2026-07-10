/*
 *  Copyright 2021-2025 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

/* If OT or BLE is enabled, the vApplicationHook defined by the app should be used
 * instead of the private definition of WIFI, use CONFIG_COEX_APP macro to select.
 */
#define CONFIG_COEX_APP                 1

#define CONFIG_WIFI_IND_DNLD 1
#define CONFIG_WIFI_IND_RESET 1

#undef CONFIG_BT_IND_DNLD
#define CONFIG_BT_IND_DNLD 1

/* OSA task: requires PRIORITY_RTOS_TO_OSA conversion */
#ifdef SERIAL_MANAGER_TASK_PRIORITY
#undef SERIAL_MANAGER_TASK_PRIORITY
#endif
#define SERIAL_MANAGER_TASK_PRIORITY (PRIORITY_RTOS_TO_OSA((CONFIG_NUM_PREEMPT_PRIORITIES - 3)))

#if defined(WIFI_IW610_BOARD_MURATA_2LL_M2) || \
    defined(WIFI_IW612_BOARD_MURATA_2EL_M2) || \
    defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || \
    defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#define CONFIG_COEX_ENABLE_PM_MENU          0