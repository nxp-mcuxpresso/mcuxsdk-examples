/** @file host_sleep.c
 *
 *  @brief Host sleep file
 *
 *  Copyright 2026 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "host_sleep.h"

#if CONFIG_HOST_SLEEP

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_common.h"
#include "fsl_power.h"
#include "fsl_gpio.h"
#include "fsl_reset.h"
#include "fsl_clock.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "wlan.h"
#include <osa.h>

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern bool wlan_is_manual;

static void (*wlan_host_sleep_pre_cfg)(void);
static void (*wlan_host_sleep_post_cfg)(void);

static volatile int s_wakeup_by = 0;
#define WAKEUP_BY_WLAN 0x1
#define WAKEUP_BY_PIN  0x4

/*******************************************************************************
 * ISR handlers
 ******************************************************************************/

/* ISR for user wakeup button: SW5 -> GPIO0_10, GPIO00_IRQn */
void GPIO00_IRQHandler(void)
{
    POWER_ModuleExitLPRequest(APP_WAKEUP_LP_REQ);
    GPIO_GpioClearInterruptFlags(APP_WAKEUP_GPIO, 1U << APP_WAKEUP_GPIO_PIN);
    DisableDeepSleepIRQ(APP_WAKEUP_IRQ);
    CLOCK_DisableClock(APP_WAKEUP_CLOCK_EN);
    s_wakeup_by = WAKEUP_BY_PIN;
    SDK_ISR_EXIT_BARRIER;
}

/* ISR for WiFi wakeup: WL_WAKE_HOST -> PIO7_4 (coord B24) -> GPIO7_4, GPIO70_IRQn */
void GPIO70_IRQHandler(void)
{
    GPIO_GpioClearInterruptFlags(APP_WIFI_WAKEUP_GPIO, 1U << APP_WIFI_WAKEUP_GPIO_PIN);
    DisableDeepSleepIRQ(APP_WIFI_WAKEUP_IRQ);
    s_wakeup_by = WAKEUP_BY_WLAN;
    SDK_ISR_EXIT_BARRIER;
}

/*******************************************************************************
 * Code
 ******************************************************************************/
static void APP_SetWakeupConfig(void)
{
    gpio_pin_config_t pin_config = {kGPIO_DigitalInput, 0U};

    /* User wakeup button: SW5 (GPIO0, pin 10) */
    RESET_ClearPeripheralReset(APP_WAKEUP_RESET_RSTn);
    CLOCK_EnableClock(APP_WAKEUP_CLOCK_EN);
    GPIO_SetPinInterruptConfig(APP_WAKEUP_GPIO, APP_WAKEUP_GPIO_PIN, kGPIO_InterruptFallingEdge);
    GPIO_PinInit(APP_WAKEUP_GPIO, APP_WAKEUP_GPIO_PIN, &pin_config);
    EnableDeepSleepIRQ(APP_WAKEUP_IRQ);
    if (POWER_ModuleEnterLPRequest(APP_WAKEUP_LP_REQ) != kStatus_Success)
    {
        PRINTF("Error: Failure to enter LP mode!\r\n");
        return;
    }

    /* WiFi wakeup: WL_WAKE_HOST (PIO7_4, GPIO7, pin 4) */
    GPIO_SetPinInterruptConfig(APP_WIFI_WAKEUP_GPIO, APP_WIFI_WAKEUP_GPIO_PIN, kGPIO_InterruptFallingEdge);
    GPIO_PinInit(APP_WIFI_WAKEUP_GPIO, APP_WIFI_WAKEUP_GPIO_PIN, &pin_config);
    GPIO_GpioClearInterruptFlags(APP_WIFI_WAKEUP_GPIO, 1U << APP_WIFI_WAKEUP_GPIO_PIN);
    EnableDeepSleepIRQ(APP_WIFI_WAKEUP_IRQ);
}

void mcu_suspend(void)
{
    if (!wlan_is_manual)
    {
        PRINTF("Error: Manual mode is not selected!\r\n");
        return;
    }

    /* Send HS_CFG to firmware only if STA/UAP is connected */
    if (is_sta_connected()
#if UAP_SUPPORT
        || is_uap_started()
#endif
    )
    {
        if (wlan_host_sleep_pre_cfg)
        {
            wlan_host_sleep_pre_cfg();
        }
    }

    APP_SetWakeupConfig();

    POWER_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);

    if (s_wakeup_by == WAKEUP_BY_PIN)
        PRINTF("Woken up by PIN (SW5)\r\n");

    s_wakeup_by = 0;

    if (is_sta_connected()
#if UAP_SUPPORT
        || is_uap_started()
#endif
    )
    {
        if (wlan_host_sleep_post_cfg)
        {
            wlan_host_sleep_post_cfg();
        }
    }
}

int hostsleep_init(void (*wlan_hs_pre_cfg)(void), void (*wlan_hs_post_cfg)(void))
{
    wlan_host_sleep_pre_cfg  = wlan_hs_pre_cfg;
    wlan_host_sleep_post_cfg = wlan_hs_post_cfg;
    return 0;
}

#endif /* CONFIG_HOST_SLEEP */
