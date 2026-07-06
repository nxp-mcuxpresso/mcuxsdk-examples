/*
 *  Copyright 2024-2025 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_open_config.h"

/* Task priority */
#if (CONFIG_NCP_BLE)
#undef CONFIG_WIFI_MAX_PRIO
#define CONFIG_WIFI_MAX_PRIO (configMAX_PRIORITIES - 6)
#endif
