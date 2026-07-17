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
#include "fsl_pmu.h"
#include "fsl_soc_src.h"
#include "fsl_gpc.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "cli.h"
#include "wlan.h"
#include <osa.h>

GPIO_HANDLE_DEFINE(s_WakeupGpioHandle);
GPIO_HANDLE_DEFINE(s_WifiWakeupGpioHandle);

static void (*wlan_host_sleep_pre_cfg)(void);
static void (*wlan_host_sleep_post_cfg)(void);
extern bool wlan_is_manual;
/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t g_savedPrimask;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void GPC_EnableWakeupSource(uint32_t irq)
{
    GPC_CM_EnableIrqWakeup(GPC_CPU_MODE_CTRL_0, irq, true);
}

void GPC_DisableWakeupSource(uint32_t irq)
{
    GPC_CM_EnableIrqWakeup(GPC_CPU_MODE_CTRL_0, irq, false);
}

void APP_WAKEUP_Callback(void *param)
{
    if ((1U << APP_WAKEUP_GPIO_PIN) & GPIO_GetPinsInterruptFlags(APP_WAKEUP_GPIO))
    {
        /* Disable interrupt. */
        GPIO_DisableInterrupts(APP_WAKEUP_GPIO, 1U << APP_WAKEUP_GPIO_PIN);
        GPIO_ClearPinsInterruptFlags(APP_WAKEUP_GPIO, 1U << APP_WAKEUP_GPIO_PIN);
        GPC_DisableWakeupSource(APP_WAKEUP_IRQ);
    }
    SDK_ISR_EXIT_BARRIER;
}

/* ISR callback for PCAL6524 INT# (GPIO3_26) */
static void APP_WIFI_WAKEUP_Callback(void *param)
{
    if ((1U << APP_WIFI_WAKEUP_GPIO_PIN) & GPIO_GetPinsInterruptFlags(APP_WIFI_WAKEUP_GPIO))
    {
        /* Disable interrupt. */
        GPIO_DisableInterrupts(APP_WIFI_WAKEUP_GPIO, 1U << APP_WIFI_WAKEUP_GPIO_PIN);
        GPIO_ClearPinsInterruptFlags(APP_WIFI_WAKEUP_GPIO, 1U << APP_WIFI_WAKEUP_GPIO_PIN);
        GPC_DisableWakeupSource(APP_WIFI_WAKEUP_IRQ);
    }
    SDK_ISR_EXIT_BARRIER;
}

void GPC_DisableAllWakeupSource(GPC_CPU_MODE_CTRL_Type *base)
{
    uint8_t i;

    for (i = 0; i < GPC_CPU_MODE_CTRL_CM_IRQ_WAKEUP_MASK_COUNT; i++)
    {
        base->CM_IRQ_WAKEUP_MASK[i] |= 0xFFFFFFFF;
    }
}

void APP_SetWakeupConfig(void)
{
    pcal6524_handle_t *handle = BOARD_GetPCAL6524Handle();
    PCAL6524_SetPinInterruptConfig(handle, BOARD_PCAL6524_WIFI_WAKE_B, kPCAL6524_IntEdgeFalling);

    hal_gpio_pin_config_t sw_config = {
        kHAL_GpioDirectionIn,
        0,
        APP_WAKEUP_GPIO_PORT,
        APP_WAKEUP_GPIO_PIN,
    };

    HAL_GpioInit(s_WakeupGpioHandle, &sw_config);
    HAL_GpioSetTriggerMode(s_WakeupGpioHandle, APP_WAKEUP_INTTERUPT_TYPE);
    HAL_GpioInstallCallback(s_WakeupGpioHandle, APP_WAKEUP_Callback, NULL);

    GPIO_ClearPinsInterruptFlags(APP_WAKEUP_GPIO, 1U << APP_WAKEUP_GPIO_PIN);
    /* Enable GPIO pin interrupt */
    GPIO_EnableInterrupts(APP_WAKEUP_GPIO, 1U << APP_WAKEUP_GPIO_PIN);
    /* Enable the Interrupt */
    EnableIRQ(APP_WAKEUP_IRQ);

    /* WiFi Wake via PCAL6524 INT# (GPIO3_26) */
    hal_gpio_pin_config_t wifi_wake_config = {
        kHAL_GpioDirectionIn,
        0,
        APP_WIFI_WAKEUP_GPIO_PORT,
        APP_WIFI_WAKEUP_GPIO_PIN,
    };

    HAL_GpioInit(s_WifiWakeupGpioHandle, &wifi_wake_config);
    HAL_GpioSetTriggerMode(s_WifiWakeupGpioHandle, kHAL_GpioInterruptFallingEdge);
    HAL_GpioInstallCallback(s_WifiWakeupGpioHandle, APP_WIFI_WAKEUP_Callback, NULL);

    GPIO_ClearPinsInterruptFlags(APP_WIFI_WAKEUP_GPIO, 1U << APP_WIFI_WAKEUP_GPIO_PIN);
    GPIO_EnableInterrupts(APP_WIFI_WAKEUP_GPIO, 1U << APP_WIFI_WAKEUP_GPIO_PIN);
    EnableIRQ(APP_WIFI_WAKEUP_IRQ);

    /* Mask all interrupt first */
    GPC_DisableAllWakeupSource(GPC_CPU_MODE_CTRL_0);
    /* Enable GPC interrupt */
    GPC_EnableWakeupSource(APP_WAKEUP_IRQ);
    /* PCAL6524 INT# */
    GPC_EnableWakeupSource(APP_WIFI_WAKEUP_IRQ);
}

AT_QUICKACCESS_SECTION_CODE(void SystemEnterSleepMode(gpc_cpu_mode_t cpuMode));
void SystemEnterSleepMode(gpc_cpu_mode_t cpuMode)
{
    assert(cpuMode != kGPC_RunMode);

    g_savedPrimask = DisableGlobalIRQ();
    __DSB();
    __ISB();

    if (cpuMode == kGPC_WaitMode)
    {
        /* Clear the SLEEPDEEP bit to go into sleep mode (WAIT) */
        SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    }
    else /* STOP and SUSPEND mode */
    {
        /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    }
    /* WFI instruction will start entry into WAIT/STOP mode */
    __WFI();

    EnableGlobalIRQ(g_savedPrimask);
    __DSB();
    __ISB();
}

void CpuModeTransition(void)
{
    GPC_CM_SetNextCpuMode(GPC_CPU_MODE_CTRL_0, kGPC_WaitMode);
    GPC_CM_EnableCpuSleepHold(GPC_CPU_MODE_CTRL_0, true);

    GPC_CPU_MODE_CTRL_0->CM_NON_IRQ_WAKEUP_MASK |=
        GPC_CPU_MODE_CTRL_CM_NON_IRQ_WAKEUP_MASK_EVENT_WAKEUP_MASK_MASK |
        GPC_CPU_MODE_CTRL_CM_NON_IRQ_WAKEUP_MASK_DEBUG_WAKEUP_MASK_MASK; /* Mask debugger wakeup */

    GPC_CM_RequestStandbyMode(GPC_CPU_MODE_CTRL_0, kGPC_WaitMode);
    SystemEnterSleepMode(kGPC_WaitMode);
}
void mcu_suspend()
{
    if (!wlan_is_manual)
    {
        PRINTF("Error: Maunal mode is not selected!\r\n");
        return;
    }

    if (wlan_host_sleep_pre_cfg)
    {
        wlan_host_sleep_pre_cfg();
    }
    APP_SetWakeupConfig();
    CpuModeTransition();
    if (wlan_host_sleep_post_cfg)
    {
        wlan_host_sleep_post_cfg();
    }
}

int hostsleep_init(void (*wlan_hs_pre_cfg)(void), void (*wlan_hs_post_cfg)(void))
{
    wlan_host_sleep_pre_cfg = wlan_hs_pre_cfg;
    wlan_host_sleep_post_cfg = wlan_hs_post_cfg;

    return 0;
}

#endif
