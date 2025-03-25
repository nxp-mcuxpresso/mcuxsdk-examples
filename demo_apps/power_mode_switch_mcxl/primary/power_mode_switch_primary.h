/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _POWER_MODE_SWITCH
#define _POWER_MODE_SWITCH

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum _app_power_mode_select
{
    kAPP_PowerModeMin = 'A' - 1,
    kAPP_PowerModeActive,
    kAPP_PowerModeSleep,
    kAPP_PowerModeDeepSleep,
    kAPP_PowerModePowerDown1,
    kAPP_PowerModePowerDown2,
    kAPP_PowerModeDeepPowerDown1,
    kAPP_PowerModeDeepPowerDown2,
    kAPP_PowerModeDeepPowerDown3,
    kAPP_PowerModeShutDown,
    kAPP_PowerModeMax,
} app_power_mode_select_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/

#endif /* _POWER_MODE_SWITCH */
