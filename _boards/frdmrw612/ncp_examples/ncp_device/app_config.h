/*
 *  Copyright 2024 ~ 2026 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "wifi_bt_module_config.h"

#if defined(WIFI_BOARD_FRDM_RW61X)

#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW_rw610.h"
#else
#error "Please define macro for RW610 board"
#endif

#ifdef __MCUXPRESSO
#include "app_mcuxpresso_config.h"
#endif

/*NCP config*/
#define CONFIG_NCP    1
#define CONFIG_NCP_DEBUG            0
#define CONFIG_CRC32_HW_ACCELERATE  1
#define CONFIG_NCP_MDNS_ENABLE      1

/* On FRDM-RW612 boards, the Xtal32k is present but not connected by default
 * An hardware rework is needed to connect the Xtal32k, so disable this flag with caution */
#ifndef gBoardUseFro32k_d
#define gBoardUseFro32k_d 1
#endif

/* CONFIG_COEX_APP will decide whether WIFI uses its own application
 * idle hook in freertos, to avoid duplication defined by the app.
 */
#if (CONFIG_NCP_WIFI && !CONFIG_NCP_BLE && !CONFIG_NCP_OT)
#define CONFIG_COEX_APP    0
#else
#define CONFIG_COEX_APP    1
#endif

#define CONFIG_MONOLITHIC_WIFI     CONFIG_NCP_WIFI

#define CONFIG_MONOLITHIC_BLE      ((CONFIG_NCP_BLE) && (!CONFIG_NCP_OT))
#define CONFIG_MONOLITHIC_BLE_15_4 CONFIG_NCP_OT

#if ((CONFIG_MONOLITHIC_WIFI) || (CONFIG_MONOLITHIC_BLE) || (CONFIG_MONOLITHIC_BLE_15_4))
#define CONFIG_SOC_SERIES_RW6XX_REVISION_A2 1
#endif

#if CONFIG_MONOLITHIC_BLE || CONFIG_MONOLITHIC_BLE_15_4
/** If set gPlatformMonolithicApp_d to 1, enable monolithic feature for loading CPU2 FW automatically */
#define gPlatformMonolithicApp_d        1

/** these macro are added to avoid build error (Error[Li006]) when using IAR compiler */
#define WIFI_FW_ADDRESS 0

#if CONFIG_MONOLITHIC_BLE
#define COMBO_FW_ADDRESS 0
#endif

#if CONFIG_MONOLITHIC_BLE_15_4
#define BLE_FW_ADDRESS 0
#endif

#endif /* CONFIG_MONOLITHIC_BLE || CONFIG_MONOLITHIC_BLE_15_4 */

#define CONFIG_SDU_OFFSET_PRIORITY 2
#define IMU_TASK_PRIORITY (PRIORITY_RTOS_TO_OSA((CONFIG_NUM_PREEMPT_PRIORITIES - 3)))
#define SERIAL_MANAGER_TASK_PRIORITY (PRIORITY_RTOS_TO_OSA((CONFIG_NUM_PREEMPT_PRIORITIES - 2)))

#define CONFIG_HOST_SLEEP           1
#define CONFIG_POWER_MANAGER        1

//#if (CONFIG_NCP_WIFI)
#define CONFIG_APP_NOTIFY_DEBUG   1
#include "wifi_config.h"
//#endif

//#if (CONFIG_NCP_BLE)
#include "ble_config.h"
//#endif

#if !defined(__GNUC__)
#if defined(CONFIG_NCP_OT) && (CONFIG_NCP_OT == 1)
#error "Ot ncp feature only supports arm gcc compilation"
#endif
#endif

#endif /* APP_CONFIG_H */
