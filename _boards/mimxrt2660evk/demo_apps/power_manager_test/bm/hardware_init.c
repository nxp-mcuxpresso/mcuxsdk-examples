/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"

#include "fsl_pm_core.h"
#include "fsl_pm_device.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_power.h"
#include "fsl_lptmr.h"
/*${header:end}*/

/*${variable:start}*/
extern pm_handle_t g_pmHandle;

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify1) = {
    .notifyCallback = APP_ControlCallback_notify,
    .data           = NULL,
};

/*! @brief VBAT LPTMR wakeup source object, registered via PM_InitWakeupSource() and
 *         armed/disarmed dynamically by APP_LptmrStart()/APP_LptmrStop() using the
 *         PM_EnableWakeupSource()/PM_DisableWakeupSource() API. AT_ALWAYS_ON_DATA
 *         because the pm_handle's wakeup source list holds a pointer to this struct
 *         across the sleep/wakeup cycle. */
AT_ALWAYS_ON_DATA(pm_wakeup_source_t g_lptmrWakeupSource);

/*! @brief Power mode name strings, indexed by PM_LP_STATE_*. Used by the notify callback. */
static const char *const s_pmStateName[PM_LP_STATE_COUNT] = {
    [PM_LP_STATE_WFI]             = "WFI",
    [PM_LP_STATE_SLEEP]           = "Sleep",
    [PM_LP_STATE_DEEP_SLEEP]      = "Deep Sleep",
    [PM_LP_STATE_POWER_DOWN]      = "Power Down",
    [PM_LP_STATE_DEEP_POWER_DOWN] = "Deep Power Down",
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree (incl. the boot ramp to HP run mode),
     * TRDC, and Debug Console init -- same as power_mode_switch_rt2660's board glue. */
    BOARD_CommonSetting();
}

status_t APP_ControlCallback_notify(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    const char *stateName = (powerState < (uint8_t)PM_LP_STATE_COUNT) ?
                            s_pmStateName[powerState] : "Unknown";

    if (eventType == kPM_EventEnteringSleep)
    {
        if (powerState >= PM_LP_STATE_POWER_DOWN)
        {
            /* Power Down and Deep Power Down do not return -- wakeup triggers a PoR reboot. */
            PRINTF("Entering %s mode. Note: wakeup will cause a reset.\r\n", stateName);
        }
        else
        {
            PRINTF("Entering %s mode.\r\n", stateName);
        }
    }
    else
    {
        /* Only reached for WFI / Sleep / Deep Sleep (returning modes). */
        PRINTF("Exiting %s mode.\r\n", stateName);
    }

    return kStatus_Success;
}

void APP_RegisterNotify(void)
{
    PM_RegisterNotify(kPM_NotifyGroup0, &g_notify1);
}

/*
 * Register the VBAT LPTMR both as the framework's low-power timer (via
 * PM_RegisterTimerController) and as a wakeup source object (via PM_InitWakeupSource).
 * The timer controller ensures PM_EnterLowPower() arms/disarms the LPTMR automatically
 * for every non-WFI state through APP_LptmrStart/Stop. The wakeup source object lets
 * APP_LptmrStart/Stop use the framework's PM_EnableWakeupSource/PM_DisableWakeupSource
 * API (which internally calls PM_DEV_ManageWakeupSource -> POWER_EnableWakeupSource +
 * EnableIRQ) rather than driving the driver layer directly from board code.
 */
void APP_InitWakeupSource(void)
{
    PM_RegisterTimerController(&g_pmHandle, APP_LptmrStart, APP_LptmrStop, NULL, NULL);
    /* enable=false: the wakeup source is not armed at init; APP_LptmrStart arms it
     * on each low-power entry via PM_EnableWakeupSource(). */
    PM_InitWakeupSource(&g_lptmrWakeupSource, (uint32_t)kPOWER_WakeupIrq_VbatLptmr, NULL, false);
}

uint32_t APP_GetWakeupTimeout(void)
{
    return APP_LPTMR_TIMEOUT_MS * 1000U; /* ms -> us */
}

/*
 * VBAT LPTMR start/stop.  The LPTMR lives in the always-on VBAT domain and is clocked
 * from ULP32K (FRO32K/OSC32K), not a CGU root clock, so it keeps counting even in Deep
 * Power Down.  Configures the compare value from the framework-supplied timeoutUs (already
 * adjusted for the selected state's exitLatency -- see fsl_pm_core.c PM_RecordAndStartTimer()),
 * then arms the wakeup source via the PM framework API.
 *
 * NOTE: VBATCON_EnableLPTMRClockGate(base, true) *gates* (stops) the clock per the driver
 * contract, so pass false to let it run -- matches power_mode_switch_rt2660's APP_CfgLptmr.
 */
void APP_LptmrStart(uint64_t timeoutUs)
{
    lptmr_config_t cfg;

    VBATCON_EnableLPTMRClockGate(VBAT__VBATCON, false);
    (void)VBATCON_SwitchUlp32kSource(VBAT__VBATCON, kVBATCON_Ulp32kSourceFro32k, 0xFFFFU);

    /* Full reset before re-arming: after a Deep Power Down wake (a PoR) the normal
     * Sleep/Deep Sleep tear-down (LPTMR_Deinit) never runs, leaving the LPTMR in a "used"
     * state where a later LPTMR_Init/StartTimer fails to re-set CSR.TEN. Deinit first to
     * force a clean start every arm, then re-ungate (Deinit gates the clock). */
    LPTMR_Deinit(APP_VBAT_LPTMR_BASE);
    VBATCON_EnableLPTMRClockGate(VBAT__VBATCON, false);

    LPTMR_GetDefaultConfig(&cfg);
    cfg.timerMode = kLPTMR_TimerModeTimeCounter;
    LPTMR_Init(APP_VBAT_LPTMR_BASE, &cfg);
    LPTMR_SetTimerPeriod(APP_VBAT_LPTMR_BASE, (uint32_t)USEC_TO_COUNT(timeoutUs, APP_LPTMR_CLK_HZ));
    LPTMR_EnableInterrupts(APP_VBAT_LPTMR_BASE, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(APP_VBAT_LPTMR_BASE);

    /* Arm wakeup source via framework API: PM_DEV_ManageWakeupSource() handles both
     * POWER_EnableWakeupSource() and EnableIRQ() in one call. */
    (void)PM_EnableWakeupSource(&g_lptmrWakeupSource);
}

void APP_LptmrStop(void)
{
    /* Disarm wakeup source via framework API: PM_DEV_ManageWakeupSource() handles both
     * POWER_DisableWakeupSource() and DisableIRQ() in one call. */
    (void)PM_DisableWakeupSource(&g_lptmrWakeupSource);
    LPTMR_StopTimer(APP_VBAT_LPTMR_BASE);
    LPTMR_Deinit(APP_VBAT_LPTMR_BASE);
}

void APP_SetConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case PM_LP_STATE_WFI:
        {
            PM_SetConstraints(PM_LP_STATE_WFI, APP_WFI_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_SLEEP:
        {
            PM_SetConstraints(PM_LP_STATE_SLEEP, APP_SLEEP_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_DEEP_SLEEP:
        {
            PM_SetConstraints(PM_LP_STATE_DEEP_SLEEP, APP_DEEP_SLEEP_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_POWER_DOWN:
        {
            PM_SetConstraints(PM_LP_STATE_POWER_DOWN, APP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_DEEP_POWER_DOWN:
        {
            PM_SetConstraints(PM_LP_STATE_DEEP_POWER_DOWN, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        default:
        {
            /* This branch will never be hit. */
            break;
        }
    }
}

void APP_ReleaseConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case PM_LP_STATE_WFI:
        {
            PM_ReleaseConstraints(PM_LP_STATE_WFI, APP_WFI_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_SLEEP:
        {
            PM_ReleaseConstraints(PM_LP_STATE_SLEEP, APP_SLEEP_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_DEEP_SLEEP:
        {
            PM_ReleaseConstraints(PM_LP_STATE_DEEP_SLEEP, APP_DEEP_SLEEP_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_POWER_DOWN:
        {
            PM_ReleaseConstraints(PM_LP_STATE_POWER_DOWN, APP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_DEEP_POWER_DOWN:
        {
            PM_ReleaseConstraints(PM_LP_STATE_DEEP_POWER_DOWN, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        default:
        {
            /* This branch will never be hit. */
            break;
        }
    }
}

/*
 * VBAT LPTMR IRQ handler: acknowledge the compare flag only. Stopping/deiniting the
 * timer is owned by APP_LptmrStop(), called from PM_StopAndRecordTimer() after wakeup
 * (Sleep/Deep Sleep/Power Down) or on the next boot's re-init (Deep Power Down PoR).
 */
void APP_VBAT_LPTMR_IRQ_HANDLER(void)
{
    LPTMR_ClearStatusFlags(APP_VBAT_LPTMR_BASE, kLPTMR_TimerCompareFlag);
    (void)PM_DisableWakeupSource(&g_lptmrWakeupSource);
    LPTMR_StopTimer(APP_VBAT_LPTMR_BASE);
    LPTMR_Deinit(APP_VBAT_LPTMR_BASE);
    SDK_ISR_EXIT_BARRIER;
}
/*${function:end}*/
