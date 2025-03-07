/*
 *  Copyright 2025 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"

#define FLASH_ADAPTER_SIZE 0x4000

#if (defined(WIFI_IW416_BOARD_AW_AM510_ARDUINO) || defined(WIFI_IW610_BOARD_MURATA_2LL_M2))
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif
