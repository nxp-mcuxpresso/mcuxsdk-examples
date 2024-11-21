/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_pm_device.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_cmc.h"
#include "fsl_pm_core.h"
#include "fsl_lptmr.h"
#include "fsl_lpuart.h"
#include "fsl_port.h"
#include "fsl_spc.h"
#include "fsl_vbat.h"
#include "fsl_port.h"
#include <setjmp.h>
/*${header:end}*/

/*${variable:start}*/
#if (defined(__ICCARM__))
extern uint32_t m_wakeup_stack_end;
extern uint32_t m_context_save_start;
#endif

extern pm_handle_t g_pmHandle;
extern jmp_buf g_coreContext;
/*${variable:end}*/

/*${function:start}*/
AT_ALWAYS_ON_DATA(pm_wakeup_source_t g_lptmr0WakeupSource);

void BOARD_InitHardware(void)
{
    BOARD_BootClockPLL180M();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    
    /* Disable debugger in low-power modes. */
    CMC_EnableDebugOperation(CMC, true);
    
    SPC_SetLowPowerWakeUpDelay(SPC0, 0x7DU);

    if ((SPC_GetRequestedLowPowerMode(SPC0) & (kSPC_PowerDownWithSysClockOff | kSPC_DeepPowerDownWithSysClockOff)) != 0UL)
    {
        SPC_ClearPeriphIOIsolationFlag(SPC0);
    }
}

static status_t APP_UartControl(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    if (eventType == kPM_EventEnteringSleep)
    {
        PRINTF("\r\n De-init UART.");
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        DbgConsole_Deinit();
        CLOCK_DisableClock(kCLOCK_GateLPUART2);
        CLOCK_DisableClock(kCLOCK_GatePORT2);
    }
    else
    {
        BOARD_BootClockPLL180M();
        BOARD_InitDEBUG_UARTPins();
        BOARD_InitDebugConsole();
        PRINTF("\r\n Re-init UART.");
    }

    return kStatus_Success;
}

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify0) = {
    .notifyCallback = APP_UartControl,
    .data           = NULL,
};

/*! @brief Wakeup source service function, should be executed if the corresponding wakeup event occurred. */
void APP_Lptmr0WakeupService(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR0))
    {
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    }
}

void LPTMR0_IRQHandler(void)
{
    PM_TriggerWakeSourceService(&g_lptmr0WakeupSource);
}

/*! @brief Start lptmr and ungate FRO16K before entering low power mode. */
void APP_StartLptmr(uint64_t timeOutTickes)
{
    const lptmr_config_t DEMO_LPTMR_config = {.timerMode            = kLPTMR_TimerModeTimeCounter,
                                              .pinSelect            = kLPTMR_PinSelectInput_0,
                                              .pinPolarity          = kLPTMR_PinPolarityActiveHigh,
                                              .enableFreeRunning    = false,
                                              .bypassPrescaler      = true,
                                              .prescalerClockSource = kLPTMR_PrescalerClock_1,
                                              .value                = kLPTMR_Prescale_Glitch_0};

    VBAT_EnableFRO16k(VBAT0, true);
    while(!VBAT_CheckFRO16kEnabled(VBAT0))
    {
    }
    VBAT_UngateFRO16k(VBAT0, VBAT_FROCLKE_CLKE(1U));

    LPTMR_Init(LPTMR0, &DEMO_LPTMR_config);
    LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(timeOutTickes, 16384UL));
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(LPTMR0);
}

/*! @brief Stop lptmr and gate FRO16K after exit low power mode. */
void APP_StopLptmr(void)
{
    LPTMR_StopTimer(LPTMR0);
    VBAT_GateFRO16k(VBAT0, VBAT_FROCLKE_CLKE(1U));
    VBAT_EnableFRO16k(VBAT0, false);
    while(VBAT_CheckFRO16kEnabled(VBAT0))
    {
    }
}

/*! @brief Init wakeup source, insert it to the wakeup source list, if the wakeup source is the lptmr,
 *  also need to register the timer controller to the power manager handle.
 */
void APP_InitWakeupSource(void)
{
    PM_InitWakeupSource(&g_lptmr0WakeupSource, PM_WSID_INT_LPTMR, APP_Lptmr0WakeupService, true);
    PM_RegisterTimerController(&g_pmHandle, APP_StartLptmr, APP_StopLptmr, NULL, NULL);
}

uint32_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;
    uint32_t timeoutTicks;

    while (1)
    {
        PRINTF("\r\n Select the wake up timeout in seconds.");
        PRINTF("\r\n The allowed range is 1s - 9s.");
        PRINTF("\r\n Eg. enter 5 to wake up in 5 seconds.");
        PRINTF("\r\n Waiting for input timeout value...");

        timeout = GETCHAR();
        if ((timeout > '0') && (timeout <= '9'))
        {
            timeout -= '0';
            PRINTF("\r\n Will wakeup in %d seconds.", timeout);
            timeoutTicks = (timeout * 1000000UL);
            return timeoutTicks;
        }
        PRINTF("\r\n Wrong value!");
    }
}

void APP_RegisterNotify(void)
{
    if (kStatus_PMSuccess != PM_RegisterNotify(kPM_NotifyGroup0, &g_notify0))
    {
        assert(false);
        PRINTF("\r\n Register notify0 failed");
    }
}

void APP_SetConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case 0: /* sleep mode */
        {
            PM_SetConstraints(PM_LP_STATE_SLEEP, APP_SLEEP_CONSTRAINTS);
            break;
        }

        case 1: /* deep sleep mode */
        {
            PM_SetConstraints(PM_LP_STATE_DEEP_SLEEP, APP_DEEP_SLEEP_CONSTRAINTS);
            break;
        }

        case 2: /* power down mode */
        {
            PM_SetConstraints(PM_LP_STATE_POWER_DOWN, APP_POWER_DOWN_CONSTRAINTS);
            break;
        }

        case 3: /* deep power down mode */
        {
            PM_SetConstraints(PM_LP_STATE_DEEP_POWER_DOWN, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }

        default:
        {
            /* This branch will never be hit. */
            assert(false);
            break;
        }
    }
}

void APP_ReleaseConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case 0: /* sleep mode */
        {
            PM_ReleaseConstraints(PM_LP_STATE_SLEEP, APP_SLEEP_CONSTRAINTS);
            break;
        }
        case 1: /* deep sleep mode */
        {
            PM_ReleaseConstraints(PM_LP_STATE_DEEP_SLEEP, APP_DEEP_SLEEP_CONSTRAINTS);
            break;
        }
        case 2: /* power down mode */
        {
            PM_ReleaseConstraints(PM_LP_STATE_POWER_DOWN, APP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        case 3: /* deep power down mode */
        {
            PM_ReleaseConstraints(PM_LP_STATE_DEEP_POWER_DOWN, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        default:
        {
            /* This branch should never be hit. */
            assert(false);
            break;
        }
    }
}

/*${function:end}*/
