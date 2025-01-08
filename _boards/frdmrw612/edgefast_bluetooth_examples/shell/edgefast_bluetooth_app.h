/*
 *  Copyright 2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config_Gen.h"
#include "edgefast_bluetooth_debug_config_Gen.h"
#include "edgefast_bluetooth_extension_config_Gen.h"

#if 0
#undef CONFIG_BT_EXT_ADV
#define CONFIG_BT_EXT_ADV 1
#undef CONFIG_BT_PER_ADV
#define CONFIG_BT_PER_ADV 1
#endif

#if 0
#define CONFIG_BT_SMP_SELFTEST 1

#undef CONFIG_BT_DEBUG
#define CONFIG_BT_DEBUG 1

#define CONFIG_NET_BUF_LOG        1
#define CONFIG_NET_BUF_POOL_USAGE 1
#endif

#undef CONFIG_BT_CLASSIC

#define SHELL_BUFFER_SIZE 512
#define SHELL_MAX_ARGS    20

#include "monolithic_config.h"


