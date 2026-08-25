/*
 *  Copyright 2025-2026 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */
 
/**
 * @file app_mcuxpresso_config.h
 * @brief Application configuration for MCUXpresso IDE.
 *
 * This header defines feature selection macros for different radio stacks
 * (Wi-Fi, BLE, Thread) and related components such as WPA Supplicant.
 */

#ifndef __APP_MCUXPRESSO_CONFIG_H__
#define __APP_MCUXPRESSO_CONFIG_H__

#ifdef __MCUXPRESSO

/* --------------------------------------------------------------------------
 * Radio feature selection
 * Multiple selections are allowed. Set to 1 to enable, 0 to disable.
 * -------------------------------------------------------------------------- */

#ifndef CONFIG_WIFI
#define CONFIG_WIFI     1
#endif

#ifndef CONFIG_BLE
#define CONFIG_BLE      0
#endif

#ifndef CONFIG_OT
#define CONFIG_OT       0
#endif

/* --------------------------------------------------------------------------
 * WPA Supplicant configuration
 * Enabled only if Wi-Fi is enabled.
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
#define CONFIG_WIFI_BLE_COEX_APP    CONFIG_WIFI
#define CONFIG_DISABLE_BLE          (!CONFIG_BLE)
#define CONFIG_OT_CLI               CONFIG_OT
#define CONFIG_WPA_SUPP_MBEDTLS     CONFIG_WPA_SUPPLICANT

#endif /* __MCUXPRESSO */
#endif /* __APP_MCUXPRESSO_CONFIG_H__ */
