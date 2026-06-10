/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MODE_SWITCH_
#define _POWER_MODE_SWITCH_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 'A' - 1,
    kAPP_PowerModeActive,               /*!< Normal RUN mode. */
    kAPP_PowerModeSleep,                /*!< Sleep. */
    kAPP_PowerModeDeepSleep,            /*!< DeepSleep */
    kAPP_PowerModePowerDown,            /*!< PowerDown */
    kAPP_PowerModeDeepPowerDown,        /*!< DeepPowerDown. */
    kAPP_PowerModeMax
} app_power_mode_t;

typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceTimer = 'A',       /*!< Wakeup by wakeup timer. */
    kAPP_WakeupSourceButton,            /*!< Wakeup by Wakeup pin. */
} app_wakeup_source_t;

extern bool g_isTimerWakeupSourceSelected;

/*! Low power modes are a contiguous subset of app_power_mode_t: Sleep .. (Max - 1)
 *  Some modules (e.g. resource control tables) need a dense 0..N-1 index.
 */
#define APP_LOW_POWER_MODE_COUNT ((uint8_t)kAPP_PowerModeMax - (uint8_t)kAPP_PowerModeSleep)

#endif /*_POWER_MODE_SWITCH_*/
