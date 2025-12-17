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

/* BIS */

