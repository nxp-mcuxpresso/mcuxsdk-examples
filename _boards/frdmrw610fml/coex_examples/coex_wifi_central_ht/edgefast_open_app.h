/*
 *  Copyright 2026 NXP
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

/* On FRDM-RW610FML boards, the Xtal32k is present but not connected by default
 * An hardware rework is needed to connect the Xtal32k, so disable this flag with caution */
#ifndef gBoardUseFro32k_d
#define gBoardUseFro32k_d 1
#endif

#endif
