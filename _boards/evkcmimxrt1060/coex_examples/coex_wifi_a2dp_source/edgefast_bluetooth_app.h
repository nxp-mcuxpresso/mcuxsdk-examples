/*
 *  Copyright 2025 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"

#include "app_config.h"

#define SHELL_BUFFER_SIZE 512U
#if ((defined(CONFIG_BT_MAP_MCE) && (CONFIG_BT_MAP_MCE > 0U)) || \
     (defined(CONFIG_BT_PBAP_PCE) && (CONFIG_BT_PBAP_PCE > 0U)))
#define SHELL_MAX_ARGS    40
#else
#define SHELL_MAX_ARGS    20
#endif

#define CONTROLLER_INIT_ESCAPE                 1