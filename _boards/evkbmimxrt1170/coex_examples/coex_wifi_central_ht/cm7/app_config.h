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

#if defined(WIFI_IW610_BOARD_MURATA_2LL_M2)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#define CONFIG_COEX_ENABLE_PM_MENU          0