/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#include "pin_mux.h"
#include "fsl_io_mux.h"
#include "board.h"
#include "FreeRTOS.h"
#include "fsl_component_serial_manager.h"
#include "mflash_common.h"
#if CONFIG_WPA_SUPP
#include "mcux_psa_els_pkc_common_init.h"
#endif

#include "osa.h"
#include "board_lp.h"
#include "ncp_crc.h"
#include "PWR_Interface.h"
#include "fsl_pm_core.h"
#include "fsl_power.h"
#include "ncp_intf_pm.h"
#include "ncp_pm_sm.h"
#include "ncp_pm_os.h"
#include "ncp_pm.h"
#include "ncp_cmd_common.h"
#include "ncp_log.h"

NCP_LOG_MODULE_DECLARE(ncp_pm);

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)
#ifndef PM_WAKEUP_SRC_PRIORITY
#define PM_WAKEUP_SRC_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#endif /* PM_WAKEUP_SRC_PRIORITY */
#else
#ifndef PM_WAKEUP_SRC_PRIORITY
#define PM_WAKEUP_SRC_PRIORITY (3U)
#endif /* PM_WAKEUP_SRC_PRIORITY */
#endif /* configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY */

#define NCP_PM_OS_TIMER_DURATION_MS (10U)

#if configUSE_TICKLESS_IDLE
/* Tickless idle is allowed by default but can be disabled runtime with APP_SetTicklessIdle */
static int ticklessIdleAllowed = 1;
#endif /* configUSE_TICKLESS_IDLE */

typedef struct _clock_context
{
    uint32_t selA;
    uint32_t selB;
} clock_context_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Save/restore context instance used when switching clocks. */
static clock_context_t s_clock_ctx;
static pm_notify_state_t pm_notify_state = {0};
static pm_wakeup_source_t gpioWakeupSource;
OSA_TIMER_HANDLE_DEFINE(lp_timer);

static ncp_pm_wake_src_t s_wake_src = NCP_PM_WAKE_SRC_NONE;
uint64_t rtc_timeout = 0;

#if CONFIG_NCP_WIFI
extern status_t powerManager_WlanNotify(pm_event_type_t eventType, uint8_t powerState, void *data);
AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t ncp_wlan_notify) =
{
    .notifyCallback = powerManager_WlanNotify,
    .data           = &pm_notify_state,
};
#endif

static status_t powerManager_PmNotify(pm_event_type_t eventType, uint8_t powerState, void *data);
AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t ncp_pm_notify) = {
    .notifyCallback = powerManager_PmNotify,
    .data           = &pm_notify_state,
};

static const serial_manager_lowpower_critical_CBs_t gSerMgr_LowpowerCriticalCBs = {
    .serialEnterLowpowerCriticalFunc = &PWR_LowPowerEnterCritical,
    .serialExitLowpowerCriticalFunc  = &PWR_LowPowerExitCritical,
};

static const ncp_pm_constraint_cbs_t s_ncp_pm_constraint_cbs = {
    .set_constraint     = &PWR_LowPowerEnterCritical,
    .release_constraint = &PWR_LowPowerExitCritical,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

void GPIO_INTA_DriverIRQHandler(void)
{
}

void PIN1_INT_IRQHandler(void)
{
}

void ncp_pm_init_wakeup_source(void *ws, uint32_t wsId, bool enable)
{
    PM_InitWakeupSource((pm_wakeup_source_t *)ws, wsId, NULL, enable);
}

void ncp_pm_enable_wakeup_source(void *ws)
{
    PM_EnableWakeupSource((pm_wakeup_source_t *)ws);
}

void ncp_pm_disable_wakeup_source(void *ws)
{
    pm_wakeup_source_t *pm_ws = (pm_wakeup_source_t *)ws;
    uint32_t irqn;
    uint32_t misc;

    assert(ws != NULL);
    PM_DECODE_WAKEUP_SOURCE_ID(pm_ws->wsId);
    (void)misc;

    if ((IRQn_Type)irqn == GPIO_INTA_IRQn)
    {
        NVIC_ClearPendingIRQ(GPIO_INTA_IRQn);
    }

    PM_DisableWakeupSource(pm_ws);
}

void ncp_pm_get_wakeup_source(void *ws)
{
    pm_wakeup_source_t *pm_ws = (pm_wakeup_source_t *)ws;
    uint32_t irqn;
    uint32_t misc;

    assert(ws != NULL);
    PM_DECODE_WAKEUP_SOURCE_ID(pm_ws->wsId);
    (void)misc;

    s_wake_src = NCP_PM_WAKE_SRC_NONE;

    if (POWER_GetWakeupStatus(RTC_IRQn))
    {
        s_wake_src = NCP_PM_WAKE_SRC_RTC;
        return;
    }

    if (POWER_GetWakeupStatus((IRQn_Type)irqn))
    {
        switch ((IRQn_Type)irqn)
        {
            case FLEXCOMM0_IRQn:
                s_wake_src = NCP_PM_WAKE_SRC_FLEXCOMM0;
                break;
            case USB_IRQn:
                s_wake_src = NCP_PM_WAKE_SRC_USB;
                break;
            case WKDEEPSLEEP_IRQn:
                s_wake_src = NCP_PM_WAKE_SRC_WKDEEPSLEEP;
                break;
            default:
                break;
        }
    }

    if (s_wake_src == NCP_PM_WAKE_SRC_NONE)
    {
        if (POWER_GetWakeupStatus(GPIO_INTA_IRQn))
        {
            s_wake_src = NCP_PM_WAKE_SRC_GPIOA;
            return;
        }
        if (POWER_GetWakeupStatus(PIN1_INT_IRQn))
        {
            s_wake_src = NCP_PM_WAKE_SRC_PIN1;
            return;
        }
        if (POWER_GetWakeupStatus(WL_MCI_WAKEUP0_IRQn))
        {
            s_wake_src = NCP_PM_WAKE_SRC_WLAN;
            return;
        }
        if (POWER_GetWakeupStatus(BLE_MCI_WAKEUP0_IRQn))
        {
            s_wake_src = NCP_PM_WAKE_SRC_BLE;
            return;
        }
   }
}

static void ncp_pm_print_wakeup_source(void)
{
    const char *src_str = "UNKNOWN";

    switch (s_wake_src)
    {
        case NCP_PM_WAKE_SRC_FLEXCOMM0:
            src_str = "FLEXCOMM0";
            break;
        case NCP_PM_WAKE_SRC_USB:
            src_str = "USB";
            break;
        case NCP_PM_WAKE_SRC_WKDEEPSLEEP:
            src_str = "WKDEEPSLEEP";
            break;
        case NCP_PM_WAKE_SRC_GPIOA:
            src_str = "GPIOA";
            break;
        case NCP_PM_WAKE_SRC_PIN1:
            src_str = "PIN1";
            break;
        case NCP_PM_WAKE_SRC_RTC:
            src_str = "RTC";
            break;
        case NCP_PM_WAKE_SRC_WLAN:
            src_str = "WLAN";
            break;
        case NCP_PM_WAKE_SRC_BLE:
            src_str = "BLE";
            break;
        case NCP_PM_WAKE_SRC_NONE:
        default:
            break;
    }

    NCP_LOG_INF("Wakeup source: %s", src_str);
}

static void ncp_pm_device_gpio_init(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t gpio_in_config = {kGPIO_DigitalInput, 0};
    gpio_pin_config_t gpio_out_config = {kGPIO_DigitalOutput, 0};
    gpio_interrupt_config_t gpio_lp_int_config = {
        .mode = kGPIO_PinIntEnableEdge,
        .polarity = kGPIO_PinIntEnableLowOrFall
    };

#if CONFIG_NCP_UART || CONFIG_NCP_USB
    GPIO_PortInit(GPIO, 0);
#endif
    GPIO_PortInit(GPIO, 1);

    if(strcmp(BOARD_NAME, "FRDM-RW612") == 0)
    {
        /* Initialize GPIO functionality on GPIO42 */
        GPIO_PinInit(GPIO, 1U, 10U, &gpio_in_config);
        GPIO_SetPinInterruptConfig(GPIO, 1U, 10U, &gpio_lp_int_config);
        GPIO_PinEnableInterrupt(GPIO, 1U, 10U, (uint32_t)kGPIO_InterruptA);
        NVIC_ClearPendingIRQ(GPIO_INTA_IRQn);
        EnableIRQ(GPIO_INTA_IRQn);
        /* Keep GPIO 42 in high level to avoid unexpected level change in PM3. */
        IO_MUX_SetPinOutLevelInSleep(42U, IO_MUX_SleepPinLevelHigh);
    }
    else if (strcmp(BOARD_NAME, "RD-RW612-BGA") == 0)
    {
#if defined(BOARD_SW4_GPIO)
        GPIO_PinInit(GPIO, BOARD_SW4_GPIO_PORT, BOARD_SW4_GPIO_PIN, &gpio_in_config);
#endif
    }

#if CONFIG_NCP_UART
    /* Keep USART0 TX (GPIO 3) output high in sleep mode to avoid unexpected signal */
    IO_MUX_SetPinOutLevelInSleep(3U, IO_MUX_SleepPinLevelHigh);
#endif

    /* Keep GPIO 50 in pull down state to void unexpected level change after enter PM3 */
    IO_MUX_SetPinConfig(50U, IO_MUX_PinConfigPullDown);
    /* Init output GPIO50 */
    GPIO_PinInit(GPIO, 1, 18, &gpio_out_config);
}

void ncp_pm_gpio_wakeup_peer(void)
{
    ncp_pm_cfg_t *power_cfg = ncp_pm_get_config();

    if (power_cfg->wakeup_host && s_wake_src == NCP_PM_WAKE_SRC_WLAN)
    {
        /* GPIO50 */
        GPIO_PortToggle(GPIO, 1, 0x40000);
        ncp_pm_delay_us(10);
        GPIO_PortToggle(GPIO, 1, 0x40000);
    }
}

AT_QUICKACCESS_SECTION_CODE(static void ncp_pm_switch_pre_hook(uint32_t mode, void *param))
{
    uint32_t freq = CLK_XTAL_OSC_CLK / 40U; //frequency of LPOSC

    /* In PM2, only LPOSC and CLK32K are available. To use interface as wakeup source,
       We have to use main_clk as system clock source, and main_clk comes from LPOSC.
       Use register access directly to avoid possible flash access in function call */
    clock_context_t * clk_ctx = (clock_context_t *)param;
    clk_ctx->selA = CLKCTL0->MAINCLKSELA;
    clk_ctx->selB = CLKCTL0->MAINCLKSELB;

    /* Switch main_clk to LPOSC */
    CLKCTL0->MAINCLKSELA = 2;
    CLKCTL0->MAINCLKSELB = 0;

    /* Update system core clock */
    SystemCoreClock = freq / ((CLKCTL0->SYSCPUAHBCLKDIV & CLKCTL0_SYSCPUAHBCLKDIV_DIV_MASK) + 1U);
}

AT_QUICKACCESS_SECTION_CODE(static void ncp_pm_switch_post_hook(uint32_t mode, void *param))
{
    /* Recover main_clk clock source after wakeup.
     Use register access directly to avoid possible flash access in function call. */
    clock_context_t * clk_ctx = (clock_context_t *)param;
    CLKCTL0->MAINCLKSELA           = clk_ctx->selA;
    CLKCTL0->MAINCLKSELB           = clk_ctx->selB;
}

static void ncp_pm_hw_reinit_on_pm3(void)
{
    /* GPIO50 pin to wakeup external host */
    BOARD_InitPins_NCP();
#if CONFIG_WPA_SUPP
    CRYPTO_ReInitHardware();
#endif /*CONFIG_WPA_SUPP */
#if CONFIG_CRC32_HW_ACCELERATE
    ncp_tlv_chksum_init();
#endif /* CONFIG_CRC32_HW_ACCELERATE */
    ncp_pm_device_gpio_init();
#if CONFIG_NCP_WIFI && !CONFIG_NCP_BLE
    mflash_drv_init();
#endif
}

static status_t powerManager_PmNotify(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    pm_notify_state_t *notify_state = (pm_notify_state_t *)data;

    if (powerState < NCP_PM_STATE_PM2)
    {
        return kStatus_PMSuccess;
    }

    if (ncp_intf_pm_prep(powerState, eventType, NULL) != NCP_PM_STATUS_SUCCESS)
    {
        return kStatus_PMPowerStateNotAllowed;
    }

    if (eventType == kPM_EventEnteringSleep)
    {
        if (ncp_pm_sm_is_done() == false)
        {
            ncp_pm_sm_post_event(NCP_PM_SM_EVENT_START);
            return kStatus_PMPowerStateNotAllowed;
        }

        if (ncp_intf_pm_enter(powerState) != NCP_PM_STATUS_SUCCESS)
        {
            return kStatus_PMPowerStateNotAllowed;
        }

        ncp_pm_enable_wakeup_source(&gpioWakeupSource);

        notify_state->pm = 1;
    }
    else if (eventType == kPM_EventExitingSleep)
    {
        if (!notify_state->pm)
        {
            return kStatus_PMFail;
        }

        notify_state->pm = 0;

        if (powerState == NCP_PM_STATE_PM3)
        {
            ncp_pm_hw_reinit_on_pm3();
        }

        ncp_intf_pm_exit(powerState);

        ncp_pm_disable_wakeup_source(&gpioWakeupSource);

        SystemCoreClockUpdate();

        ncp_pm_sm_post_event(NCP_PM_SM_EVENT_STOP);

        ncp_pm_print_wakeup_source();
    }

    return kStatus_PMSuccess;
}

void ncp_pm_os_activate_lp_timer(uint32_t duration_ms)
{
    if (duration_ms > 0U)
    {
        (void)OSA_TimerChange((osa_timer_handle_t)lp_timer, duration_ms/ portTICK_PERIOD_MS, 0);

        /* Start the timer, during this time, the configured low power mode will be used as much as possible by
         * the system, when the timer expires, the low power mode will be limited to WFI until next command
         * This is to make sure the serial interface becomes available again to the user */
        if (OSA_TimerActivate((osa_timer_handle_t)lp_timer) != KOSA_StatusSuccess)
        {
            assert(0);
        }
    }
}

static void ncp_pm_os_timer_cb(osa_timer_arg_t arg)
{
    if (!ncp_pm_sm_is_idle())
    {
        ncp_pm_sm_post_event(NCP_PM_SM_EVENT_TIME_WAIT);
        return;
    }

    ncp_pm_configure_next_lowpower_mode(NCP_PM_STATE_PM1, 0U);
}

int ncp_pm_os_init(void)
{
    /* It is required to initialize PWR module so the application
    * can call PWR API during its init (wake up sources...) */
    PWR_Init();

    /* Initialize board_lp module, likely to register the enter/exit
    * low power callback to Power Manager */
    BOARD_LowPowerInit();

    IRQ_SetPriority(RTC_IRQn, PM_WAKEUP_SRC_PRIORITY);
    if(strcmp(BOARD_NAME, "FRDM-RW612") == 0)
    {
        IRQ_SetPriority(GPIO_INTA_IRQn, PM_WAKEUP_SRC_PRIORITY);
        ncp_pm_init_wakeup_source(&gpioWakeupSource, (uint32_t)GPIO_INTA_IRQn, false);
    }
    else if(strcmp(BOARD_NAME, "RD-RW612-BGA") == 0)
    {
        IRQ_SetPriority(PIN1_INT_IRQn, PM_WAKEUP_SRC_PRIORITY);
        ncp_pm_init_wakeup_source(&gpioWakeupSource, (uint32_t)PIN1_INT_IRQn, false);
    }

    ncp_intf_pm_init();

#if CONFIG_NCP_WIFI
    /* Register WLAN notifier */
    PM_RegisterNotify(kPM_NotifyGroup0, &ncp_wlan_notify);
#endif
    /* Register NCP power management notifier */
    PM_RegisterNotify(kPM_NotifyGroup1, &ncp_pm_notify);

    POWER_SetPowerSwitchCallback(ncp_pm_switch_pre_hook, &s_clock_ctx, ncp_pm_switch_post_hook, &s_clock_ctx);

    /* Set WFI constraint by default (works for All application)
    * Application will be allowed to release the WFI constraint and set a deepest lowpower mode constraint such as
    * DeepSleep or PowerDown if it needs more optimization */
    PWR_SetLowPowerModeConstraint(NCP_PM_STATE_PM0);

    /* Register PWR functions into SerialManager module in order to disable device lowpower
        during SerialManager processing. Typically, allow only WFI instruction when
        uart data are processed by serail manager  */
    SerialManager_SetLowpowerCriticalCb(&gSerMgr_LowpowerCriticalCBs);

    ncp_pm_register_lp_constraint_cbs(&s_ncp_pm_constraint_cbs);

    ncp_pm_device_gpio_init();

    if (OSA_TimerCreate((osa_timer_handle_t)lp_timer, NCP_PM_OS_TIMER_DURATION_MS, &ncp_pm_os_timer_cb, NULL,
                KOSA_TimerOnce, OSA_TIMER_NO_ACTIVATE) != KOSA_StatusSuccess)
    {
        return NCP_PM_STATUS_ERROR;
    }

    return NCP_PM_STATUS_SUCCESS;
}

void ncp_pm_os_deinit(void)
{
    (void)OSA_TimerDeactivate((osa_timer_handle_t)lp_timer);
    (void)OSA_TimerDestroy((osa_timer_handle_t)lp_timer);

#if CONFIG_NCP_WIFI
    PM_UnregisterNotify(&ncp_wlan_notify);
#endif
    PM_UnregisterNotify(&ncp_pm_notify);

    ncp_pm_unregister_lp_constraint_cbs();
}

#if configUSE_TICKLESS_IDLE
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    bool abortIdle = false;
    uint64_t expectedIdleTimeUs, actualIdleTimeUs;

    if(ticklessIdleAllowed > 0)
    {
        uint32_t irqMask = DisableGlobalIRQ();

        /* Disable and prepare systicks for low power */
        abortIdle = PWR_SysticksPreProcess((uint32_t)xExpectedIdleTime, &expectedIdleTimeUs);

        if (abortIdle == false)
        {
            /* Enter low power with a maximal timeout */
            actualIdleTimeUs = PWR_EnterLowPower(expectedIdleTimeUs);

            /* Re enable systicks and compensate systick timebase */
            PWR_SysticksPostProcess(expectedIdleTimeUs, actualIdleTimeUs);
        }

        ncp_pm_gpio_wakeup_peer();

        /* Exit from critical section */
        EnableGlobalIRQ(irqMask);
    }
    else
    {
        /* Tickless idle is not allowed, wait for next tick interrupt */
        __WFI();
    }
}

void APP_SetTicklessIdle(bool enable)
{
    if(enable == true)
    {
        ticklessIdleAllowed++;
    }
    else
    {
        ticklessIdleAllowed--;
    }
}
#endif /* configUSE_TICKLESS_IDLE */
