/*
 *  Copyright 2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"
#include "edgefast_bluetooth_audio_config.h"

#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
#undef SD_TIMING_MAX
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#endif /*#define WIFI_IW612_BOARD_MURATA_2EL_M2*/

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
#endif

/* CIS */

/* VCP */

/* MCS */

/* MPL */

/* CSIP */
#define CONFIG_LITTLE_ENDIAN 1

