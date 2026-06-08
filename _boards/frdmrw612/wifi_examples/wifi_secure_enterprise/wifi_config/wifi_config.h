/*
 *  Copyright 2020-2022 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _WIFI_CONFIG_H_
#define _WIFI_CONFIG_H_

#include "wifi_bt_module_config.h"

#define CONFIG_IPV6 1
#define CONFIG_MAX_IPV6_ADDRESSES 3

#define CONFIG_MAX_RESCAN_LIMIT 30
#define PRINTF_FLOAT_ENABLE 1
#define CONFIG_HOST_SLEEP 0
#define CONFIG_POWER_MANAGER 0
#define CONFIG_MEF_CFG 0
/** If define CONFIG_TX_RX_ZERO_COPY 1, please make sure
 *  #define PBUF_POOL_BUFSIZE 1752
 *  in lwipopts.h
 */
#define CONFIG_TX_RX_ZERO_COPY 1
#define CONFIG_ANT_DETECT 1
#define CONFIG_CSI_AMI 0
/*
 * Config options for wpa supplicant
 */
#define CONFIG_WPA_SUPP 1

#if CONFIG_WPA_SUPP
#define CONFIG_WPA_SUPP_WPS               0
#define CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE 1
#define CONFIG_WPA_SUPP_CRYPTO_AP_ENTERPRISE 0
#define CONFIG_WIFI_USB_FILE_ACCESS       0

#define CONFIG_WPA_SUPP_DPP 0
#define CONFIG_WPA_SUPP_NAN_USD 0

#if (CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE || CONFIG_WPA_SUPP_CRYPTO_AP_ENTERPRISE)
#define CONFIG_EAP_TLS 1
#define CONFIG_EAP_PEAP 0
#define CONFIG_EAP_TTLS 0
#define CONFIG_EAP_FAST 0
#define CONFIG_EAP_SIM 0
#define CONFIG_EAP_AKA 0
#define CONFIG_EAP_AKA_PRIME 0

#if (CONFIG_EAP_PEAP || CONFIG_EAP_TTLS || CONFIG_EAP_FAST)
#define CONFIG_EAP_MSCHAPV2 0
#define CONFIG_EAP_GTC 0
#endif
#endif

#define CONFIG_WPA_SUPP_P2P 0
#endif

/* WLCMGR debug */
#define CONFIG_WLCMGR_DEBUG 0

/*
 * Wifi extra debug options
 */
#define CONFIG_WIFI_EXTRA_DEBUG 0
#define CONFIG_WIFI_EVENTS_DEBUG 0
#define CONFIG_WIFI_CMD_RESP_DEBUG 0
#define CONFIG_WIFI_PKT_DEBUG 0
#define CONFIG_WIFI_SCAN_DEBUG 0
#define CONFIG_WIFI_IO_INFO_DUMP 0
#define CONFIG_WIFI_IO_DEBUG 0
#define CONFIG_WIFI_IO_DUMP 0
#define CONFIG_WIFI_MEM_DEBUG 0
#define CONFIG_WIFI_AMPDU_DEBUG 0
#define CONFIG_WIFI_TIMER_DEBUG 0
#define CONFIG_WIFI_SDIO_DEBUG 0
#define CONFIG_WIFI_FW_DEBUG 1
#define CONFIG_WIFI_UAP_DEBUG 0
#define CONFIG_WPS_DEBUG 0
#define CONFIG_FW_VDLL_DEBUG 0
#define CONFIG_DHCP_SERVER_DEBUG 0
#define CONFIG_FWDNLD_IO_DEBUG 0
#define CONFIG_WIFI_SG_DEBUG 0
#define CONFIG_WIFI_PS_DEBUG 0

/* CPU loading debug */
#define CONFIG_CPU_LOADING 0

#if CONFIG_WIFI_FW_DEBUG
#define CONFIG_FW_DUMP_FLASH_START_ADDR 0x0B000000
#define CONFIG_FW_DUMP_FLASH_ERASE_LENGTH 0x200000
#endif
/*
 * Heap debug options
 */
#define CONFIG_HEAP_DEBUG 0
#define CONFIG_HEAP_STAT 0

/*
 * wpa supplicant debug options
 */
#define CONFIG_WPA_SUPP_DEBUG_LEVEL 3

#define CONFIG_SUPP_DEBUG 0

#endif /* _WIFI_CONFIG_H_ */
