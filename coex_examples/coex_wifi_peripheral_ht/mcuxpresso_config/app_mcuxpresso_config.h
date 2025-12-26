/*
 *  Copyright 2025 NXP
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
 * Only support to enable/disable CONFIG_WPA_SUPPLICANT now.
 * -------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------
 * Radio feature selection
 * Note: 
 *      Keep CONFIG_WIFI as default value.
 * -------------------------------------------------------------------------- */

#ifndef CONFIG_WIFI
#define CONFIG_WIFI     1
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
#define CONFIG_COEX_ENABLE_WIFI_MENU    CONFIG_WIFI
#define CONFIG_WIFI_BLE_COEX_APP        CONFIG_WIFI
#define CONFIG_COEX_ENABLE_MENU         CONFIG_WIFI
#define CONFIG_COEX_ENABLE_BLE_MENU     0
#define CONFIG_DISABLE_BLE              0
#define CONFIG_OT_CLI                   0
#define CONFIG_WPA_SUPP_MBEDTLS         CONFIG_WPA_SUPPLICANT

#endif /* __MCUXPRESSO */
#endif /* __APP_MCUXPRESSO_CONFIG_H__ */
