/*
 *  Copyright 2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config_Gen.h"
#include "edgefast_bluetooth_debug_config_Gen.h"
#include "edgefast_bluetooth_extension_config_Gen.h"

/* Select witch beacon application to start */
#define BEACON_APP  1
#define IBEACON_APP 0
#define EDDYSTONE   0

#if (defined EDDYSTONE) && (EDDYSTONE)
#undef CONFIG_BT_SETTINGS
#define CONFIG_BT_SETTINGS              1
#undef CONFIG_BT_KEYS_OVERWRITE_OLDEST
#define CONFIG_BT_KEYS_OVERWRITE_OLDEST 1
#endif
#if defined(IBEACON_APP) && (IBEACON_APP == 1)
#undef CONFIG_BT_DEVICE_NAME
#define CONFIG_BT_DEVICE_NAME "ibeacon"
#elif defined(EDDYSTONE) && (EDDYSTONE == 1)
#undef CONFIG_BT_DEVICE_NAME
#define CONFIG_BT_DEVICE_NAME "eddystone"
#elif defined(BEACON_APP) && (BEACON_APP == 1)
#undef CONFIG_BT_DEVICE_NAME
#define CONFIG_BT_DEVICE_NAME "beacon"
#endif
#include "monolithic_config.h"