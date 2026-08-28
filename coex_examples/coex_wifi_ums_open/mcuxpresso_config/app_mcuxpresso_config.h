/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_MCUXPRESSO_CONFIG_H
#define APP_MCUXPRESSO_CONFIG_H

#ifdef __MCUXPRESSO

/* Feature flags for coex_wifi_ums_open */
#ifndef CONFIG_WIFI
#define CONFIG_WIFI           1
#endif

#ifndef CONFIG_BLE
#define CONFIG_BLE            1
#endif

#if CONFIG_WIFI
#ifndef CONFIG_WPA_SUPPLICANT
#define CONFIG_WPA_SUPPLICANT 1
#endif
#endif

/* Derived macros consumed by app_config.h */
#define CONFIG_WIFI_BLE_COEX_APP  CONFIG_WIFI
#define CONFIG_DISABLE_BLE        (!CONFIG_BLE)
#define CONFIG_WPA_SUPP_MBEDTLS   CONFIG_WPA_SUPPLICANT

#endif /* __MCUXPRESSO */

#endif /* APP_MCUXPRESSO_CONFIG_H */
