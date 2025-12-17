/*
 *  Copyright 2020-2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"
#include "edgefast_bluetooth_audio_config.h"

#define FLASH_ADAPTER_SIZE 0x10000

#if defined(BT_THIRD_PARTY_TRANSCEIVER) || defined(WIFI_IW612_BOARD_RD_USD) || \
    defined(WIFI_IW612_BOARD_MURATA_2EL_M2) || defined(BT_NW61X_BOARD_NXP_RD_USD)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#if defined(BT_NW61X_BOARD_NXP_RD_USD)
#define CONFIG_BT_IND_RESET 1
#define CONFIG_BT_IND_DNLD 1
#endif /*#define BT_NW61X_BOARD_NXP_RD_USD*/

#if CONFIG_BT_CSIP_SET_MEMBER
    #define CONFIG_TMAP_PERIPHERAL_DUO  0 /* Duo headset */
#endif

#if defined(CONFIG_TMAP_PERIPHERAL_DUO) && (CONFIG_TMAP_PERIPHERAL_DUO > 0)
    #define CONFIG_TMAP_PERIPHERAL_SET_RANK 1
#endif

#define CONFIG_TMAP_PERIPHERAL_LEFT  0
#define CONFIG_TMAP_PERIPHERAL_RIGHT 1

#if CONFIG_BT_BAP_UNICAST_SERVER
#undef CONFIG_BT_BAP_UNICAST
#define CONFIG_BT_BAP_UNICAST        1
#undef CONFIG_BT_ISO_UNICAST
#define CONFIG_BT_ISO_UNICAST        1
#undef CONFIG_BT_ISO_PERIPHERAL
#define CONFIG_BT_ISO_PERIPHERAL     1
#undef CONFIG_BT_GATT_CACHING
#define CONFIG_BT_GATT_CACHING       1
#undef CONFIG_BT_ASCS
#define CONFIG_BT_ASCS               1
#endif

