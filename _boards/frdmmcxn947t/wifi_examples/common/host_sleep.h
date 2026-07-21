/** @file host_sleep.h
 *
 *  @brief Host sleep file
 *
 *  Copyright 2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HOST_SLEEP_H_
#define _HOST_SLEEP_H_

#include "board.h"

#include "fsl_common.h"

#include "fsl_adapter_gpio.h"
#include "wifi_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 'A' - 1,
    kAPP_PowerModeActive,        /* Normal RUN mode. */
    kAPP_PowerModeSleep,         /* Sleep. */
    kAPP_PowerModeDeepSleep,     /* DeepSleep */
    kAPP_PowerModePowerDown,     /* PowerDown */
    kAPP_PowerModeDeepPowerDown, /* DeepPowerDown. */
    kAPP_PowerModeMax
} app_power_mode_t;

typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceLptmr = 'A', /*!< Wakeup by LPTMR. */
    kAPP_WakeupSourceButton,      /*!< Wakeup by WakeupButton. */
} app_wakeup_source_t;


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

#if CONFIG_HOST_SLEEP
int hostsleep_init(void (*wlan_hs_pre_cfg)(void), void (*wlan_hs_post_cfg)(void));
void mcu_suspend();
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /*_HOST_SLEEP_H_*/

