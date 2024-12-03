/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "fsl_pm_core.h"
#include "fsl_pm_device.h"
#include "fsl_lptmr.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_lpuart.h"
#include "fsl_port.h"
#include "fsl_cmc.h"
#include "fsl_spc.h"

/*${header:end}*/

extern pm_handle_t g_pmHandle;

/*${function:start}*/

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify1) = {
    .notifyCallback = APP_UartControlCallback,
    .data           = NULL,
};


AT_ALWAYS_ON_DATA(pm_wakeup_source_t g_lptmr0WakeupSource);

void LPTMR0_IRQHandler(void)
{
    PM_TriggerWakeSourceService(&g_lptmr0WakeupSource);
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CMC_EnableDebugOperation(CMC0, false);
}

status_t APP_UartControlCallback(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    if (eventType == kPM_EventEnteringSleep)
    {
        /* De-init uart */
        PRINTF("De-init UART.\r\n");
        /* Wait for debug console output finished. */
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        DbgConsole_Deinit();
        /*
         * Set pin for current leakage.
         * Debug console RX pin: Set to pinmux to disable.
         * Debug console TX pin: Don't need to change.
         */
        PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, kPORT_PinDisabledOrAnalog);
    }
    else
    {
        /* Re-init uart. */
        BOARD_InitPins();
        BOARD_BootClockRUN();
        BOARD_InitDebugConsole();
        if ((CMC_GetSystemResetStatus(CMC0) & kCMC_WakeUpReset) != 0UL)
        {
            /* Close ISO. */
            SPC_ClearPeriphIOIsolationFlag(SPC0);
        }
        PRINTF("Re-init UART.\r\n");
    }

    return kStatus_Success;
}

void APP_InitWakeupSource(void)
{
    PM_InitWakeupSource(&g_lptmr0WakeupSource, PM_WSID_LPTMR0, APP_Lptmr0WakeupService, true);
    PM_RegisterTimerController(&g_pmHandle, APP_StartLptmr, APP_StopLptmr, NULL, NULL);
}

void APP_Lptmr0WakeupService(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR0))
    {
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR0);
    }
}

void APP_StartLptmr(uint64_t timeOutTickes)
{
    const lptmr_config_t DEMO_LPTMR_config = {.timerMode            = kLPTMR_TimerModeTimeCounter,
                                              .pinSelect            = kLPTMR_PinSelectInput_0,
                                              .pinPolarity          = kLPTMR_PinPolarityActiveHigh,
                                              .enableFreeRunning    = false,
                                              .bypassPrescaler      = true,
                                              .prescalerClockSource = kLPTMR_PrescalerClock_2,
                                              .value                = kLPTMR_Prescale_Glitch_0};

    LPTMR_Init(APP_LPTMR, &DEMO_LPTMR_config);
    LPTMR_SetTimerPeriod(APP_LPTMR, (uint32_t)timeOutTickes);
    LPTMR_EnableInterrupts(APP_LPTMR, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(APP_LPTMR);
}

void APP_StopLptmr(void)
{
    LPTMR_StopTimer(APP_LPTMR);
}

uint32_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;
    uint32_t timeoutTicks;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");
        PRINTF("The allowed range is 1s ~ 9s.\r\n");
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");

        timeout = GETCHAR();
        PRINTF("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            timeout -= '0';
            PRINTF("Will wakeup in %d seconds.\r\n", timeout);
            timeoutTicks = (32000UL * timeout) - 1UL;
            return timeoutTicks;
        }
        PRINTF("Wrong value!\r\n");
    }
}

void APP_RegisterNotify(void)
{
    PM_RegisterNotify(kPM_NotifyGroup0, &g_notify1);
}

void APP_SetConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
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
            /* This branch should never be hit. */
            break;
        }
    }
}

/*${function:end}*/
