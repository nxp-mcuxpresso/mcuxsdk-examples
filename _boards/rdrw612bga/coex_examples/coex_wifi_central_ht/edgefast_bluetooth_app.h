/*
 *  Copyright 2020-2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"


#include "app_config.h"

/* Enable/Disable low power entry on tickless idle */
#define APP_LOWPOWER_ENABLED 1

#if defined(APP_LOWPOWER_ENABLED) && (APP_LOWPOWER_ENABLED > 0)
/* Defines the low power mode of BLE host when scanning and connecting */
#define APP_LOW_POWER_MODE      PWR_PowerDown
#endif
