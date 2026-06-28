/** @file host_sleep.h
 *
 *  @brief Host sleep file
 *
 *  Copyright 2026 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _HOST_SLEEP_H_
#define _HOST_SLEEP_H_

#include "fsl_adapter_gpio.h"
#include "wifi_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define APP_WAKEUP_GPIO           GPIO3
#define APP_WAKEUP_GPIO_PORT      3
#define APP_WAKEUP_GPIO_PIN       28
#define APP_WAKEUP_IRQ            GPIO3_Combined_16_31_IRQn
#define APP_WAKEUP_INTTERUPT_TYPE kHAL_GpioInterruptFallingEdge

/*******************************************************************************
 * Variables
 ******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

#if CONFIG_HOST_SLEEP
int hostsleep_init(void (*wlan_hs_pre_cfg)(void), void (*wlan_hs_post_cfg)(void));
void mcu_suspend();
#endif

#ifdef __cplusplus
}
#endif

#endif /*_HOST_SLEEP_H_*/
