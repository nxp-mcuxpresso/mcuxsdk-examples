/*
 *  Copyright 2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config_Gen.h"
#include "edgefast_bluetooth_debug_config_Gen.h"
#include "edgefast_bluetooth_extension_config_Gen.h"

#define SERIAL_PORT_TYPE_BLE_WU            1

#define FLASH_ADAPTER_SIZE                0x10000
#define OSA_USED                          1
#define porting_errno                     1

#ifndef BOARD_USER_BUTTON_GPIO
#define BOARD_USER_BUTTON_GPIO BOARD_SW2_GPIO_PORT
#endif
#ifndef BOARD_USER_BUTTON_GPIO_PIN
#define BOARD_USER_BUTTON_GPIO_PIN (BOARD_SW2_GPIO_PIN)
#endif
#define BOARD_USER_BUTTON_IRQ  GPIO_INTA_IRQn
#define BOARD_USER_BUTTON_NAME "SW2"

#ifndef WIFI_BOARD_RW610
#define WIFI_BOARD_RW610
#endif

#if defined(WIFI_BOARD_RW610)
#define RW610
#define WIFI_BT_USE_IMU_INTERFACE
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW_rw610.h"
#else
#error "Please define macro for RW610 board"
#endif

#include "wifi_config.h"
/* Due to the changes in wifi configuration,
   please do not use '#undef' to diasble wifi uart task. */
#define CONFIG_UART_INTERRUPT 0

#include "monolithic_config.h"