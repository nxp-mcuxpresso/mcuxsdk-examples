/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_MCUXPRESSO_CONFIG_H
#define APP_MCUXPRESSO_CONFIG_H

#ifdef __MCUXPRESSO

/* Feature flags for coex_wifi_handsfree_ag */
#define CONFIG_WIFI           1
#define CONFIG_BLE            1
#define CONFIG_OT_CLI         0

#if CONFIG_WIFI
#define CONFIG_WPA_SUPPLICANT 1
#else
#define CONFIG_WPA_SUPPLICANT 0
#endif

/* Derived macros consumed by app_config.h */
#define CONFIG_WIFI_BLE_COEX_APP  CONFIG_WIFI
#define CONFIG_DISABLE_BLE        (!CONFIG_BLE)
#define CONFIG_WPA_SUPP_MBEDTLS   CONFIG_WPA_SUPPLICANT

#endif /* __MCUXPRESSO */

#endif /* APP_MCUXPRESSO_CONFIG_H */
