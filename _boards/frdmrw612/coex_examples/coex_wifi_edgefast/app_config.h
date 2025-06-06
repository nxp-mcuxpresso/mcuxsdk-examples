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

#ifndef RW612_SERIES
#define RW612_SERIES
#endif
#define SHELL_BUFFER_SIZE 512
#define SHELL_MAX_ARGS 40

/* If OT or BLE is enabled, the vApplicationHook defined by the app should be used
 * instead of the private definition of WIFI, use CONFIG_COEX_APP macro to select.
 */
#if (CONFIG_OT_CLI || (!CONFIG_DISABLE_BLE))
#define CONFIG_COEX_APP                 1
#else
#define CONFIG_COEX_APP                 0
#endif

/* Monolithic configure */
/* Include CPU1 FW */
#if CONFIG_WIFI_BLE_COEX_APP
#define CONFIG_MONOLITHIC_WIFI              1
#else
#define CONFIG_MONOLITHIC_WIFI              0
#endif

/* Include CPU2 FW */
#if (CONFIG_OT_CLI) /* CPU2 FW using combo image (ble + 15.4) */
#define CONFIG_MONOLITHIC_BLE_15_4          1
#define CONFIG_MONOLITHIC_BLE               0
#elif ((!CONFIG_OT_CLI) && (!CONFIG_DISABLE_BLE)) /* CPU2 FW using ble only image (ble) */
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

/* On FRDM-RW612 boards, the Xtal32k is present but not connected by default
 * An hardware rework is needed to connect the Xtal32k, so disable this flag with caution */
#ifndef gBoardUseFro32k_d
#define gBoardUseFro32k_d 1
#endif

#include "wifi_config.h"
