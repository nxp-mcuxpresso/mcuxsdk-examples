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

#include "fsl_gpio.h"
#include "fsl_power.h"
#include "wifi_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* User wakeup button: SW5 -> GPIO0, pin 10, GPIO00_IRQn */
#define APP_WAKEUP_GPIO           GPIO0
#define APP_WAKEUP_GPIO_PIN       10U
#define APP_WAKEUP_IRQ            GPIO00_IRQn
#define APP_WAKEUP_LP_REQ         kPower_GPIO0_LPREQ
#define APP_WAKEUP_CLOCK_EN       kCLOCK_Gpio0
#define APP_WAKEUP_RESET_RSTn     kGPIO0_RST_SHIFT_RSTn

/* WiFi wakeup: WL_WAKE_HOST -> PIO7_4 (coord B24) -> GPIO7, pin 4, GPIO70_IRQn */
#define APP_WIFI_WAKEUP_GPIO      GPIO7
#define APP_WIFI_WAKEUP_GPIO_PIN  4U
#define APP_WIFI_WAKEUP_IRQ       GPIO70_IRQn

/* Deep sleep power domain configuration for RT700 (7-element array for POWER_EnterDeepSleep).
 * Matches USB LPM example configuration which is verified to work on FRDM-RT700. */
#define APP_DEEPSLEEP_SLEEPCFG                                                                                  \
    (SLEEPCON0_SLEEPCFG_PLLLDO_PD_MASK | SLEEPCON0_SLEEPCFG_PLLANA_PD_MASK | SLEEPCON0_SLEEPCFG_LPOSC_PD_MASK | \
     SLEEPCON0_SLEEPCFG_COMP_MAINCLK_SHUTOFF_MASK | SLEEPCON0_SLEEPCFG_FRO1_PD_MASK |                           \
     SLEEPCON0_SLEEPCFG_FRO0_PD_MASK | SLEEPCON0_SLEEPCFG_FRO0_GATE_MASK)
#define APP_DEEPSLEEP_PDSLEEPCFG0 (PMC_PDSLEEPCFG0_VNCOM_DSR_MASK | PMC_PDSLEEPCFG0_V2NMED_DSR_MASK)
#define APP_DEEPSLEEP_RAM_APD     (0x3FFFFU) /* Keep all RAM powered */
#define APP_DEEPSLEEP_RAM_PPD     (0U)
#define APP_DEEPSLEEP_PDSLEEPCFG4 \
    (PMC_PDSLEEPCFG4_CPU0_CCACHE_MASK | PMC_PDSLEEPCFG4_CPU0_SCACHE_MASK | PMC_PDSLEEPCFG4_OCOTP_MASK)
#define APP_DEEPSLEEP_PDSLEEPCFG5 (0U)

#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                          \
    (((const uint32_t[]){APP_DEEPSLEEP_SLEEPCFG, APP_DEEPSLEEP_PDSLEEPCFG0, 0U, APP_DEEPSLEEP_RAM_APD,     \
                         APP_DEEPSLEEP_RAM_PPD, APP_DEEPSLEEP_PDSLEEPCFG4, APP_DEEPSLEEP_PDSLEEPCFG5}))

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#if CONFIG_HOST_SLEEP
int hostsleep_init(void (*wlan_hs_pre_cfg)(void), void (*wlan_hs_post_cfg)(void));
void mcu_suspend(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _HOST_SLEEP_H_ */
