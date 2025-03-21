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

#include "app_bluetooth_config.h"
#include "wifi_config.h"

#define CONFIG_WIFI_IND_DNLD 1
#define CONFIG_WIFI_IND_RESET 1

#undef CONFIG_BT_IND_DNLD
#define CONFIG_BT_IND_DNLD 1
