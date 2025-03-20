/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"
#include "edgefast_bluetooth_audio_config.h"

#define FLASH_ADAPTER_SIZE 0x10000

#if defined(BT_THIRD_PARTY_TRANSCEIVER) || defined(WIFI_IW612_BOARD_RD_USD) || defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

/* re-define CONFIG_BT_A2DP=1 and CONFIG_BT_A2DP_SINK=1 to enable bridge mode! */
#if CONFIG_BT_A2DP
#undef CONFIG_BT_A2DP
#define CONFIG_BT_A2DP 0
#endif

#if CONFIG_BT_A2DP_SINK
#undef CONFIG_BT_A2DP_SINK
#define CONFIG_BT_A2DP_SINK 0
#endif

#if CONFIG_BT_CENTRAL_ONLY
#undef CONFIG_BT_CENTRAL_ONLY
#define CONFIG_BT_CENTRAL_ONLY 0
#endif

/* This configuration used to upsample 44.1k audio to 48k in a2dp bridge. */
#ifndef APP_BRIDGE_UPSAMPLE_441_TO_48
#define APP_BRIDGE_UPSAMPLE_441_TO_48 0
#endif

#if CONFIG_BT_A2DP_SINK
#undef CONFIG_BT_DEVICE_NAME
#define CONFIG_BT_DEVICE_NAME           "a2dp_bridge"
#else
#undef CONFIG_BT_DEVICE_NAME
#define CONFIG_BT_DEVICE_NAME           "unicast_media_sender"
#endif

#if CONFIG_BT_A2DP_SINK
#undef CONFIG_BT_MAX_CONN
#define CONFIG_BT_MAX_CONN              3
#undef CONFIG_BT_MAX_PAIRED
#define CONFIG_BT_MAX_PAIRED            3
#undef CONFIG_BT_ID_MAX
#define CONFIG_BT_ID_MAX                3
#else
#undef CONFIG_BT_MAX_CONN
#define CONFIG_BT_MAX_CONN              2
#undef CONFIG_BT_MAX_PAIRED
#define CONFIG_BT_MAX_PAIRED            2
#undef CONFIG_BT_ID_MAX
#define CONFIG_BT_ID_MAX                2
#endif

#if CONFIG_BT_A2DP_SINK
#undef CONFIG_BT_PERIPHERAL
#define CONFIG_BT_PERIPHERAL  1
#undef CONFIG_BT_CENTRAL_ONLY
#define CONFIG_BT_CENTRAL_ONLY 1
#endif

/* CIS */

/* VCP */

/* MCS */

/* MPL */

/* CSIP */
#define CONFIG_LITTLE_ENDIAN 1

