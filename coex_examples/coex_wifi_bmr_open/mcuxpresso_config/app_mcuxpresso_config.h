/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_MCUXPRESSO_CONFIG_H
#define APP_MCUXPRESSO_CONFIG_H

#ifdef __MCUXPRESSO

/* --------------------------------------------------------------------------
 * Radio feature selection
 * Multiple selections are allowed. Set to 1 to enable, 0 to disable.
 * -------------------------------------------------------------------------- */

#ifndef CONFIG_WIFI
#define CONFIG_WIFI     1
#endif

#ifndef CONFIG_BLE
#define CONFIG_BLE      1
#endif

/* --------------------------------------------------------------------------
 * Derived configuration macros
 * These depend on the above feature selections.
 * -------------------------------------------------------------------------- */

#if CONFIG_WIFI
#ifndef CONFIG_WPA_SUPPLICANT
#define CONFIG_WPA_SUPPLICANT 1
#endif
#endif /* CONFIG_WIFI */

/* --------------------------------------------------------------------------
 * Derived configuration macros
 * These depend on the above feature selections.
 * -------------------------------------------------------------------------- */
#define CONFIG_WIFI_BLE_COEX_APP  CONFIG_WIFI
#define CONFIG_DISABLE_BLE        (!CONFIG_BLE)
#define CONFIG_WPA_SUPP_MBEDTLS   CONFIG_WPA_SUPPLICANT

#endif /* __MCUXPRESSO */

#endif /* APP_MCUXPRESSO_CONFIG_H */
