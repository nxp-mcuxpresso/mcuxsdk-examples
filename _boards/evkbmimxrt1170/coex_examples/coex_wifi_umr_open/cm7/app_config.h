/*
 *  Copyright 2026 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#if (CONFIG_OT_CLI || (!CONFIG_DISABLE_BLE))
#define CONFIG_COEX_APP                 1
#else
#define CONFIG_COEX_APP                 0
#endif

#include "edgefast_open_config.h"
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
