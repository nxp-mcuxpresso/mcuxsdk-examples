/*
 *  Copyright 2020-2023 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _WIFI_CONFIG_H_
#define _WIFI_CONFIG_H_

#define CONFIG_WIFI_MAX_PRIO (configMAX_PRIORITIES - 1)

#ifndef RW610
#define CONFIG_MAX_AP_ENTRIES 10
#else
#define CONFIG_MAX_AP_ENTRIES 30
#endif

#if defined(SD8977) || defined(SD8978) || defined(SD8987) || defined(RW610)
#define CONFIG_5GHz_SUPPORT 1
#endif

#ifndef RW610
#define CONFIG_SDIO_MULTI_PORT_RX_AGGR 1
#endif

#if defined(SD8987) || defined(RW610)
#define CONFIG_11AC 1
#endif

#if defined(RW610)
#define PRINTF_FLOAT_ENABLE 1
#define CONFIG_11AX         1
#define CONFIG_IMU_GDMA     0
/* WMM options */
#define CONFIG_WMM          1
#define CONFIG_WMM_ENH      1
#define CONFIG_WMM_CERT     0
#define AMSDU_IN_AMPDU      0
/* OWE mode */
#define CONFIG_OWE          0
/* WLAN SCAN OPT */
#define CONFIG_SCAN_WITH_RSSIFILTER  1
/* WLAN white/black list opt */
#define CONFIG_WIFI_DTIM_PERIOD      1
#define CONFIG_UART_INTERRUPT        1
#define CONFIG_WIFI_CAPA             1
#define CONFIG_WIFI_11D_ENABLE       1
#define CONFIG_WIFI_HIDDEN_SSID      1
#define CONFIG_WMM_UAPSD             1
#define CONFIG_WIFI_GET_LOG          1
#define CONFIG_11K                   1
#define CONFIG_WIFI_TX_PER_TRACK     1
#define CONFIG_ROAMING               1
#define CONFIG_HOST_SLEEP            0
#define CONFIG_POWER_MANAGER         0
#define CONFIG_CSI                   1
#define CONFIG_WIFI_RESET            1
#define CONFIG_NET_MONITOR           1
#define CONFIG_WIFI_MEM_ACCESS       1
#define CONFIG_WIFI_REG_ACCESS       1
#define CONFIG_ECSA                  1
#define CONFIG_WIFI_EU_CRYPTO        1
#define CONFIG_EXT_SCAN_SUPPORT      1
#define CONFIG_COMPRESS_TX_PWTBL     1
#define WIFI_ADD_ON                  1
#endif

#define CONFIG_IPV6                  0
//#define CONFIG_MAX_IPV6_ADDRESSES 3

/* Logs */
#define CONFIG_ENABLE_ERROR_LOGS   1
#define CONFIG_ENABLE_WARNING_LOGS 1

/* WLCMGR debug */
#define CONFIG_WLCMGR_DEBUG        0

/*
 * Wifi extra debug options
 */
#define CONFIG_WIFI_EXTRA_DEBUG     0
#define CONFIG_WIFI_EVENTS_DEBUG    0
#define CONFIG_WIFI_CMD_RESP_DEBUG  0
#define CONFIG_WIFI_PKT_DEBUG       0
#define CONFIG_WIFI_SCAN_DEBUG      0
#define CONFIG_WIFI_IO_INFO_DUMP    0
#define CONFIG_WIFI_IO_DEBUG        0
#define CONFIG_WIFI_IO_DUMP         0
#define CONFIG_WIFI_MEM_DEBUG       0
#define CONFIG_WIFI_AMPDU_DEBUG     0
#define CONFIG_WIFI_TIMER_DEBUG     0
#define CONFIG_WIFI_SDIO_DEBUG      0
#define CONFIG_WIFI_FW_DEBUG        0

/*
 * Heap debug options
 */
#define CONFIG_HEAP_DEBUG           0
#define CONFIG_HEAP_STAT            0

#endif /* _WIFI_CONFIG_H_ */
