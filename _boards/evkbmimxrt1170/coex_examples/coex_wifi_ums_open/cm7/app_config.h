/*
 *  Copyright 2026 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#ifdef __MCUXPRESSO
#include "app_mcuxpresso_config.h"
#endif /* __MCUXPRESSO */

#if (CONFIG_OT_CLI || (!CONFIG_DISABLE_BLE))
#define CONFIG_COEX_APP                 1
#else
#define CONFIG_COEX_APP                 0
#endif

#include "edgefast_open_config.h"
#include "wifi_bt_module_config.h"
#include "wifi_config.h"

#endif /* APP_CONFIG_H */
