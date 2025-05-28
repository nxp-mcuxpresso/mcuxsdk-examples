/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _POWER_MODE_SWITCH
#define _POWER_MODE_SWITCH

#include "fsl_common.h"
#include "fsl_power.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_POWER_TRANS_COUNT 11U

char *const g_modeTransArray[APP_POWER_TRANS_COUNT] = {
    "Active --> Sleep --> Active",
    "Active --> Deep Sleep --> Active",
    "Active --> Power Down1 --> Active",
    "Active --> Power Down2 --> Active",
    "Active --> Deep Power Down1 --> Active",
    "Active --> Deep Power Down1 --> Deep Power Down2 --> Deep Power Down1 --> Active",
    "Active --> Deep Power Down1 --> Deep Power Down2 --> Active",
    "Active --> Deep Power Down2 --> Active",
    "Active --> Deep Power Down2 --> Deep Power Down1 --> Active",
    "Active --> Deep Power Down3 --> Active",
    "Active --> Shut Down --> Active",
};
uint32_t g_appPowerTrans[APP_POWER_TRANS_COUNT] = {
    (uint32_t)kPower_Sleep | (uint32_t)(kPower_Active << 4U) | 0xFFFFFF00UL,
    (uint32_t)kPower_DeepSleep | (uint32_t)(kPower_Active << 4U) | 0xFFFFFF00UL,
    (uint32_t)kPower_PowerDown1 | (uint32_t)(kPower_Active << 4U) | 0xFFFFFF00UL,
    (uint32_t)kPower_PowerDown2 | (uint32_t)(kPower_Active << 4U) | 0xFFFFFF00UL,
    (uint32_t)kPower_DeepPowerDown1 | (uint32_t)(kPower_Active << 4U) | 0xFFFFFF00UL,
    (uint32_t)kPower_DeepPowerDown1 | (uint32_t)(kPower_DeepPowerDown2 << 4U) |
        (uint32_t)(kPower_DeepPowerDown1 << 8U) | (uint32_t)(kPower_Active << 12U) | 0xFFFF0000UL,
    (uint32_t)kPower_DeepPowerDown1 | (uint32_t)(kPower_DeepPowerDown2 << 4U) | (uint32_t)(kPower_Active << 8U) |
        0xFFFFF000UL,
    (uint32_t)kPower_DeepPowerDown2 | (uint32_t)(kPower_Active << 4U) | 0xFFFFFF00UL,
    (uint32_t)kPower_DeepPowerDown2 | (uint32_t)(kPower_DeepPowerDown1 << 4U) | (uint32_t)(kPower_Active << 8U) |
        0xFFFFF000UL,
    (uint32_t)kPower_DeepPowerDown3 | (uint32_t)(kPower_Active << 4U) | 0xFFFFFF00UL,
    (uint32_t)kPower_ShutDown | (uint32_t)(kPower_Active << 4U) | 0xFFFFFF00UL,
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/

#endif /* _POWER_MODE_SWITCH */
