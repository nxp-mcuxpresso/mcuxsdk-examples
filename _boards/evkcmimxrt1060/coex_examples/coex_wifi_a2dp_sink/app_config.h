/*
 *  Copyright 2021-2025 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#define CONFIG_COEX_APP                 1

//#define ENABLE_BT_IND_RESET 1

#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2) || \
    defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || \
    defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif
