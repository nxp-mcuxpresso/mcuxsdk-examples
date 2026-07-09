/*
 *  Copyright 2020-2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_open_config.h"


#include "app_config.h"

/* Enable/Disable low power entry on tickless idle */
#define APP_LOWPOWER_ENABLED 1

#if defined(APP_LOWPOWER_ENABLED) && (APP_LOWPOWER_ENABLED > 0)
/* Defines the low power mode of BLE host when scanning and connecting */
#define APP_LOW_POWER_MODE      PWR_PowerDown

/* On RD-RW612-BGA boards, the Xtal32k is present and connected by default
 * so we use it by default. If an hardware rework done on the board disconnects
 * the Xtal32k, this flag should be set to 1 to fall back to the Fro32k */
#ifndef gBoardUseFro32k_d
#define gBoardUseFro32k_d 0
#endif

#endif
