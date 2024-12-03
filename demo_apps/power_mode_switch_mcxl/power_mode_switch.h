/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _POWER_MODE_SWITCH
#define _POWER_MODE_SWITCH

#include "fsl_common.h"
#include "fsl_ipmq.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum _app_power_mode_select
{
    kAPP_PowerModeMin = 'A' - 1,
    kAPP_PowerModeActive,
    kAPP_PowerModeSleep,
    kAPP_PowerModeDeepSleep,
    kAPP_PowerModeDeepPowerDown1,
    kAPP_PowerModeDeepPowerDown2,
    kAPP_PowerModeDeepPowerDown3,
    kAPP_PowerModeShutDown,
    kAPP_PowerModeMax,
} app_power_mode_select_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DEMO_MU_ErrorReport(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

#endif /* _POWER_MODE_SWITCH */
