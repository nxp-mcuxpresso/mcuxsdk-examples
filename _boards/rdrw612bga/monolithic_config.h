/*
 *  Copyright 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

/* This Monolithic config */

#if (defined(gPlatformMonolithicApp_d) && (gPlatformMonolithicApp_d > 0))

#ifndef RW610
#define RW610
#endif

#define CONFIG_SOC_SERIES_RW6XX_REVISION_A2     1

#ifndef CONFIG_MONOLITHIC_BLE
#define BLE_FW_ADDRESS 0
#endif // CONFIG_MONOLITHIC_BLE

#ifndef CONFIG_MONOLITHIC_BLE_15_4
#define COMBO_FW_ADDRESS 0
#endif // CONFIG_MONOLITHIC_BLE_15_4

#ifndef CONFIG_MONOLITHIC_WIFI
#define WIFI_FW_ADDRESS 0
#endif // CONFIG_MONOLITHIC_WIFI

#endif /* gPlatformMonolithicApp_d */