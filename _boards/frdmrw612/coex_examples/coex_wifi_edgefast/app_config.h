/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

/* @TEST_ANCHOR */
#define WIFI_BOARD_RW610
/* @END_TEST_ANCHOR */

#if defined(WIFI_BOARD_RW610)
#define WIFI_BT_USE_IMU_INTERFACE 1
#define RW610

#define RW612_SERIES
#define CONFIG_WIFI_BLE_COEX_APP 1
#define CONFIG_DISABLE_BLE 0
#define CONFIG_WPA_SUPP_MBEDTLS 1
#define SHELL_BUFFER_SIZE 512
#define SHELL_MAX_ARGS 40
/* Monolithic configure */
/* Include CPU1 FW */
#if CONFIG_WIFI_BLE_COEX_APP
#define CONFIG_MONOLITHIC_WIFI              1
#else
#define CONFIG_MONOLITHIC_WIFI              0
#endif

/* Include CPU2 FW */
#if ((CONFIG_OT_CLI) && !(CONFIG_DISABLE_BLE)) /* CPU2 FW using combo image (ble + 15.4) */
#define CONFIG_MONOLITHIC_BLE_15_4          1
#define CONFIG_MONOLITHIC_BLE               0
#elif !(CONFIG_DISABLE_BLE) /* CPU2 FW using ble only image (ble) */
#define CONFIG_MONOLITHIC_BLE_15_4          0
#define CONFIG_MONOLITHIC_BLE               1
#else /* Not use CPU2 FW */
#define CONFIG_MONOLITHIC_BLE_15_4          0
#define CONFIG_MONOLITHIC_BLE               0
#endif

#if (CONFIG_MONOLITHIC_BLE && CONFIG_MONOLITHIC_BLE_15_4)
#error "The case that CONFIG_MONOLITHIC_BLE and CONFIG_MONOLITHIC_BLE_15_4 both be set is now allowed"
#endif

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


#ifndef CONFIG_BT_SNOOP
    #define CONFIG_BT_SNOOP 1
#endif
