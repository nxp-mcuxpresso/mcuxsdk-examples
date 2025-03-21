/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MODE_SWITCH_
#define _POWER_MODE_SWITCH_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 'A' - 1,
    kAPP_PowerModeRun,     /* Run mode, full power mode. */
    kAPP_PowerModeStandby, /* Standby mode, low power mode. */
    kAPP_PowerModeMax
} app_power_mode_t;

typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceSWT = 'A',                      /*!< Wakeup by SWT. */
    kAPP_WakeupSourceRTCAPI,                         /*!< Wakeup by RTC API. */
    kAPP_WakeupSourceRTC,                            /*!< Wakeup by RTC timeout. */
    kAPP_WakeupSourceLPCMPRoundRobinWakeupInterrupt, /*!< Wakeup by LPCMP round robin wake-up interrupt */
    kAPP_WakeupSourceRTI,                            /*!< Wakeup by RTI. */
    kAPP_WakeupSourceButton,                         /*!< Wakeup by Wakeup button pin. */
} app_wakeup_source_t;

#endif /*_POWER_MODE_SWITCH_*/
