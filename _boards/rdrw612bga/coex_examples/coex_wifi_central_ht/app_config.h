/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

/* @TEST_ANCHOR */
#ifndef WIFI_BOARD_RW610
#define WIFI_BOARD_RW610
#endif
/* @END_TEST_ANCHOR */

#if defined(WIFI_BOARD_RW610)
#define WIFI_BT_USE_IMU_INTERFACE 1
#define RW610

#define RW612_SERIES

/* If OT or BLE is enabled, the vApplicationHook defined by the app should be used
 * instead of the private definition of WIFI, use CONFIG_COEX_APP macro to select.
 */
#define CONFIG_COEX_APP                 1

/* Monolithic configure */
#define CONFIG_MONOLITHIC_WIFI              1

#define CONFIG_MONOLITHIC_BLE               1


#if ((CONFIG_MONOLITHIC_WIFI) || (CONFIG_MONOLITHIC_BLE) || (CONFIG_MONOLITHIC_BLE_15_4))
#define CONFIG_MFG_MONOLITHIC               0
#define CONFIG_SOC_SERIES_RW6XX_REVISION_A2 1
#define gPlatformMonolithicApp_d            1
#endif

#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW_rw610.h"
#else
#error "Please define macro for RW610 board"
#endif  /* defined(WIFI_BOARD_RW610) */

#include "wifi_config.h"
