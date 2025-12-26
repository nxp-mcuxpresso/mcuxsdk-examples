/*
 *  Copyright 2025 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file app_mcuxpresso_config.h
 * @brief Configuration header for MCUXpresso-based NCP (Network Co-Processor).
 *
 * This file defines compile-time configuration options for:
 * - Radio protocols (Wi-Fi, BLE, OpenThread)
 * - Host interface selection (UART, SPI, USB, SDIO)
 * - WPA Supplicant settings (enabled only if Wi-Fi is active)
 */

#ifndef __APP_MCUXPRESSO_CONFIG_H__
#define __APP_MCUXPRESSO_CONFIG_H__

#ifdef __MCUXPRESSO

/* --------------------------------------------------------------------------
 * Radio protocol selection
 * Multiple selections allowed. Set to 1 to enable, 0 to disable.
 * -------------------------------------------------------------------------- */
#ifndef CONFIG_NCP_WIFI
#define CONFIG_NCP_WIFI     1
#endif

#ifndef CONFIG_NCP_BLE
#define CONFIG_NCP_BLE      0
#endif

#ifndef CONFIG_NCP_OT
#define CONFIG_NCP_OT       0
#endif

/* --------------------------------------------------------------------------
 * Host interface selection
 * Only one interface should be enabled at a time.
 * Set to 1 to enable, 0 to disable.
 * -------------------------------------------------------------------------- */
#ifndef CONFIG_NCP_UART
#define CONFIG_NCP_UART     1
#endif

#ifndef CONFIG_NCP_SPI
#define CONFIG_NCP_SPI      0
#endif

#ifndef CONFIG_NCP_USB
#define CONFIG_NCP_USB      0
#endif

#ifndef CONFIG_NCP_SDIO
#define CONFIG_NCP_SDIO     0
#endif

/* --------------------------------------------------------------------------
 * WPA Supplicant configuration
 * Enabled only if Wi-Fi is active.
 * -------------------------------------------------------------------------- */
#if CONFIG_NCP_WIFI
#ifndef CONFIG_WPA_SUPPLICANT
#define CONFIG_WPA_SUPPLICANT 1
#endif
#endif /* CONFIG_NCP_WIFI */

/* --------------------------------------------------------------------------
 * Derived configuration macros
 * These depend on the above feature selections.
 * -------------------------------------------------------------------------- */
#define CONFIG_WPA_SUPP_MBEDTLS     CONFIG_WPA_SUPPLICANT

#endif /* __MCUXPRESSO */
#endif /* __APP_MCUXPRESSO_CONFIG_H__ */
