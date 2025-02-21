/*
 *  Copyright 2025 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config_Gen.h"
#include "edgefast_bluetooth_debug_config_Gen.h"
#include "edgefast_bluetooth_extension_config_Gen.h"

#define SERIAL_PORT_TYPE_BLE_WU            1

#define CONTROLLER_INIT_ESCAPE             1

#define FLASH_ADAPTER_SIZE              0x4000
#define OSA_USED                        1
#define porting_errno                     1

#if defined(WIFI_IW416_BOARD_AW_AM510_ARDUINO)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#define HAL_UART_DMA_RING_BUFFER_ENABLE 0
