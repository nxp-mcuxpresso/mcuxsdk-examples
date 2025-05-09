/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MANAGER_H_
#define _POWER_MANAGER_H_

#include "fsl_common.h"
#include "fsl_notifier.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Power mode definition used in application. */
typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 'A' - 1,
    kAPP_PowerModeRun,   /*!< Run mode. */
    kAPP_PowerModeStop,  /*!< Stop mode. */
    kAPP_PowerModeVlpr,  /*!< Very low power run mode. */
    kAPP_PowerModeVlps,  /*!< Very low power stop mode. */
    kAPP_PowerModeHsrun, /*!< High-speed run mode. Chip-specific. */
    kAPP_PowerModeMax
} app_power_mode_t;

/*!
 * @brief Power mode user configuration structure.
 *
 * This structure defines power mode with additional power options and specifies
 * transition to and out of this mode. Application may define multiple power modes and
 * switch between them.
 */
typedef struct power_user_config
{
    app_power_mode_t mode; /*!< Power mode. */

} power_user_config_t;

/* callback data type which is used for power manager user callback */
typedef struct
{
    uint32_t beforeNotificationCounter; /*!< Callback before notification counter */
    uint32_t afterNotificationCounter;  /*!< Callback after notification counter */
    smc_power_state_t originPowerState; /*!< Origin power state before switch */
} user_callback_data_t;

typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceLptmr, /*!< Wakeup by LPTMR.        */
    kAPP_WakeupSourcePin    /*!< Wakeup by external pin. */
} app_wakeup_source_t;

#endif /* _POWER_MANAGER_H_ */
