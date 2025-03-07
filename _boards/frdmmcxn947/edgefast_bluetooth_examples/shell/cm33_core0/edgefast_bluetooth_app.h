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

#ifdef K32W061_TRANSCEIVER
#undef CONFIG_BT_CLASSIC
#endif

#ifdef WIFI_IW610_BOARD_MURATA_2LL_M2
#undef CONFIG_BT_CLASSIC
#undef CONFIG_BT_HFP_HF
#undef CONFIG_BT_IND_RESET
#undef CONFIG_BT_IND_RESET_OOB
#endif

#define HAL_UART_DMA_RING_BUFFER_ENABLE 0

#define SHELL_BUFFER_SIZE 512
#define SHELL_MAX_ARGS    20

