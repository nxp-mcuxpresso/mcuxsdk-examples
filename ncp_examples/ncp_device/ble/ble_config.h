/*
 *  Copyright 2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config_Gen.h"
#include "edgefast_bluetooth_debug_config_Gen.h"
#include "edgefast_bluetooth_extension_config_Gen.h"

/* Task priority */
#if (CONFIG_NCP_BLE)
#undef CONFIG_WIFI_MAX_PRIO
#define CONFIG_WIFI_MAX_PRIO (configMAX_PRIORITIES - 6)
#endif
