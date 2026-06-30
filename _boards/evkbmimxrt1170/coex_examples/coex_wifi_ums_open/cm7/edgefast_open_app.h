/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_open_config.h"

#define FLASH_ADAPTER_SIZE 0x10000
#define OSA_USED           1

#if defined(BT_THIRD_PARTY_TRANSCEIVER) || defined(WIFI_IW612_BOARD_RD_USD) ||     defined(WIFI_IW612_BOARD_MURATA_2EL_M2) || defined(BT_NW61X_BOARD_NXP_RD_USD)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2) || defined(BT_NW61X_BOARD_NXP_RD_USD)
#undef SD_TIMING_MAX
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#endif
