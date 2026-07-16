/*
 *  Copyright 2023-2026 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_open_config.h"

#include "app_config.h"

#if (defined(WIFI_IW416_BOARD_MURATA_1XK_M2) || defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || \
     defined(WIFI_IW612_BOARD_MURATA_2EL_M2))
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#define OSA_USED 1

#undef CONFIG_BT_NXP_IW610
#undef CONFIG_BT_NXP_IW416
#undef CONFIG_BT_NXP_88W8987
#undef CONFIG_BT_NXP_NW612

#if !(defined(WIFI_IW416_BOARD_MURATA_1XK_M2) || defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || \
      defined(WIFI_IW612_BOARD_MURATA_2EL_M2) || defined(BT_NW61X_BOARD_NXP_RD_USD) || \
      defined(WIFI_IW610_BOARD_MURATA_2LL_M2) || defined(WIFI_AW611_BOARD_UBX_JODY_W5_M2))
#undef CONFIG_BT_H4_NXP_CTLR
#endif

#if defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
#define CONFIG_BT_NXP_IW416
#endif /* WIFI_IW416_BOARD_MURATA_1XK_M2 */

#if defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2)
#define CONFIG_BT_NXP_88W8987
#endif /* WIFI_88W8987_BOARD_MURATA_1ZM_M2 */

#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2) || defined(BT_NW61X_BOARD_NXP_RD_USD) || \
    defined(WIFI_AW611_BOARD_UBX_JODY_W5_M2)
#define CONFIG_BT_NXP_NW612

#undef SD_TIMING_MAX
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#endif /* WIFI_IW612_BOARD_MURATA_2EL_M2 || BT_NW61X_BOARD_NXP_RD_USD ||
          WIFI_AW611_BOARD_UBX_JODY_W5_M2 */

#if defined(WIFI_IW610_BOARD_MURATA_2LL_M2)
#define CONFIG_BT_NXP_IW610
#endif /* WIFI_IW610_BOARD_MURATA_2LL_M2 */

#if !(defined(CONFIG_BT_NXP_NW612) || defined(CONFIG_BT_NXP_IW416))
#undef CONFIG_HCI_NXP_SET_CAL_DATA
#undef CONFIG_HCI_NXP_SET_CAL_DATA_ANNEX100
#endif /* !(CONFIG_BT_NXP_NW612 || CONFIG_BT_NXP_IW416) */
