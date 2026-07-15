/*
 *  Copyright 2023, 2026 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Supported Wi-Fi boards (modules):
 *     WIFI_88W8977_BOARD_PAN9026_SDIO
 *     WIFI_88W8977_BOARD_AW_AM281_USD
 *     WIFI_IW416_BOARD_AW_AM457_USD
 *     WIFI_IW416_BOARD_AW_AM457MA
 *     WIFI_IW416_BOARD_AW_AM510_USD
 *     WIFI_IW416_BOARD_AW_AM510MA
 *     WIFI_88W8987_BOARD_AW_CM358_USD
 *     WIFI_88W8987_BOARD_AW_CM358MA
 *     WIFI_IW416_BOARD_MURATA_1XK
 *     WIFI_88W8987_BOARD_MURATA_1ZM
 *     WIFI_BOARD_RW610
 */

#include "wifi_board.h"

/* Wi-Fi boards configuration list */

/* Panasonic PAN9026 SDIO ADAPTER */
#if defined(WIFI_88W8977_BOARD_PAN9026_SDIO)
#define SD8977
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#define SD_CLOCK_MAX (25000000U)

/* Azurewave AW-AM281-uSD */
#elif defined(WIFI_88W8977_BOARD_AW_AM281_USD)
#define SD8977

/* AzureWave AW-AM457-uSD/AW-AM457MA */
#elif defined(WIFI_IW416_BOARD_AW_AM457_USD) || defined(WIFI_IW416_BOARD_AW_AM457MA)
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_1V8

/* AzureWave AW-AM510-uSD/AW-AM510MA */
#elif defined(WIFI_IW416_BOARD_AW_AM510_USD) || defined(WIFI_IW416_BOARD_AW_AM510MA)
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_1V8

/* AzureWave AW-CM358-uSD/AW-CM358MA */
#elif defined(WIFI_88W8987_BOARD_AW_CM358_USD) || defined(WIFI_88W8987_BOARD_AW_CM358MA)
#define SD8987
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode

/* Murata 1XK */
#elif defined(WIFI_IW416_BOARD_MURATA_1XK)
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define HOST_PDN_RESET

/* Murata 1ZM */
#elif defined(WIFI_88W8987_BOARD_MURATA_1ZM)
#define SD8987
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define HOST_PDN_RESET

/* RW610 */
#elif defined(WIFI_BOARD_RW610)
#define RW610
#define WIFI_BT_USE_IMU_INTERFACE
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW_rw610.h"

/* FRDMRW61X */
#elif defined(WIFI_BOARD_FRDM_RW61X)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW_rw610.h"
#define RW610
#define FRDMRW610
#define WIFI_BT_USE_IMU_INTERFACE
#define CONFIG_SOC_SERIES_RW6XX_REVISION_A2 1

/* 2EL Firecrest module with M2 interface */
#elif defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_CA_RU_Tx_power.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_EU_RU_Tx_power.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_JP_RU_Tx_power.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_US_RU_Tx_power.h"
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_WW.h"
#define SD9177
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0xA, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xA \
    }

#else
#error "Please define macro related to wifi board"
#endif

#include "wifi_config.h"
