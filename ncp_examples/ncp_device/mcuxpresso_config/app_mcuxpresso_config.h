/*
 *  Copyright 2025 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_MCUXPRESSO_CONFIG_H__
#define __APP_MCUXPRESSO_CONFIG_H__

#ifdef __MCUXPRESSO
/* 
    Radios: multiple selections allowed.
*/
#ifndef CONFIG_NCP_WIFI
#define CONFIG_NCP_WIFI     1
#endif

#ifndef CONFIG_NCP_BLE
#define CONFIG_NCP_BLE      0
#endif

#ifndef CONFIG_NCP_OT
#define CONFIG_NCP_OT       0
#endif

/* 
    Host Interface: only one selection allowed.
*/
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

/* 
    ===== WPA Supplicant (enabled only if WIFI is enabled) ===== 
*/
#if CONFIG_NCP_WIFI

#ifndef CONFIG_WPA_SUPP_MBEDTLS
#define CONFIG_WPA_SUPP_MBEDTLS 1
#endif

#endif /* CONFIG_NCP_WIFI */

#endif /* __MCUXPRESSO */
#endif /* __APP_MCUXPRESSO_CONFIG_H__ */
