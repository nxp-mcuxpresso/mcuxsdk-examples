/*
 * Copyright 2022-2023 NXP
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
#include "fsl_rtc.h"
#include "fsl_power.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_usart.h"
/*${header:end}*/

extern pm_handle_t g_pmHandle;

/*${function:start}*/

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify1) = {
    .notifyCallback = APP_UartControlCallback,
    .data           = NULL,
};

AT_ALWAYS_ON_DATA(pm_wakeup_source_t g_OstimerWakeupSource);

AT_ALWAYS_ON_DATA_INIT(power_init_config_t g_initCfg) = {
    .iBuck         = true, /* VCORE AVDD18 supplied from iBuck on RD board. */
    .gateCauRefClk = true, /* CAU_SOC_SLP_REF_CLK not needed. */
};

extern void RTC_IRQHandler(void);
void RTC_IRQHandler()
{
    if (RTC_GetStatusFlags(RTC) & kRTC_AlarmFlag)
    {
        /* Clear alarm flag */
        RTC_ClearStatusFlags(RTC, kRTC_AlarmFlag);
        POWER_ClearWakeupStatus(RTC_IRQn);
        PRINTF("Woken up by RTC\r\n");
    }
}

void BOARD_InitHardware(void)
{
    uint32_t resetSrc;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitSleepPinConfig();
    POWER_InitPowerConfig(&g_initCfg);

    resetSrc = POWER_GetResetCause();
    PRINTF("\r\nMCU wakeup source 0x%x...\r\n", resetSrc);
    /* In case PM4 wakeup, the wakeup config and status need to be cleared */
    POWER_ClearResetCause(resetSrc);
    DisableIRQ(RTC_IRQn);
    POWER_ClearWakeupStatus(RTC_IRQn);
    POWER_DisableWakeup(RTC_IRQn);

    RTC_Init(RTC);
    /* Enable wakeup in PD mode */
    RTC_EnableAlarmTimerInterruptFromDPD(RTC, true);
    /* Start RTC */
    RTC_ClearStatusFlags(RTC, kRTC_AlarmFlag);
    RTC_StartTimer(RTC);
}

status_t APP_UartControlCallback(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    if (powerState >= PM_LP_STATE_PM2 && powerState <= PM_LP_STATE_PM3)
    {
        if (eventType == kPM_EventEnteringSleep)
        {
            /* De-init uart */
            PRINTF("De-init UART.\r\n");
            /* Wait for debug console output finished. */
            while (((uint32_t)kUSART_TxFifoEmptyFlag & USART_GetStatusFlags(BOARD_DEBUG_UART)) == 0U)
            {
            }
            DbgConsole_Deinit();
        }
        else
        {
            if (powerState == PM_LP_STATE_PM3)
            {
                /* Perihperal state lost, need reinitialize in exit from PM3 */
                BOARD_InitBootPins();
                BOARD_InitBootClocks();
                POWER_InitPowerConfig(&g_initCfg);
            }
            /* Re-init uart. */
            BOARD_InitDebugConsole();
            PRINTF("Re-init UART.\r\n");
        }
    }

    return kStatus_Success;
}

void APP_InitWakeupSource(void)
{
    PM_InitWakeupSource(&g_OstimerWakeupSource, (uint32_t)RTC_IRQn, NULL, true);
    PM_RegisterTimerController(&g_pmHandle, APP_StartRtcTimer, APP_StopRtcTimer, NULL, NULL);
}

void APP_StartRtcTimer(uint64_t timeOutUs)
{
    uint32_t currSeconds;

    /* Read the RTC seconds register to get current time in seconds */
    currSeconds = RTC_GetSecondsTimerCount(RTC);
    /* Add alarm seconds to current time */
    currSeconds += (uint32_t)((timeOutUs + 999999U) / 1000000U);
    /* Set alarm time in seconds */
    RTC_SetSecondsTimerMatch(RTC, currSeconds);
}

void APP_StopRtcTimer(void)
{
    /* Do nothing */
}

uint32_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;

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
            return (uint32_t)timeout * 1000000UL;
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
        case PM_LP_STATE_PM2:
        {
            PM_SetConstraints(powerMode, APP_PM2_CONSTRAINTS);
            break;
        }

        case PM_LP_STATE_PM3:
        {
            PM_SetConstraints(powerMode, APP_PM3_CONSTRAINTS);
            break;
        }

        case PM_LP_STATE_PM4:
        {
            PM_SetConstraints(powerMode, APP_PM4_CONSTRAINTS);
            break;
        }

        default:
        {
            /* PM0/PM1 has no reousrce constraints. */
            PM_SetConstraints(powerMode, 0U);
            break;
        }
    }
}

void APP_ReleaseConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case PM_LP_STATE_PM2:
        {
            PM_ReleaseConstraints(powerMode, APP_PM2_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_PM3:
        {
            PM_ReleaseConstraints(powerMode, APP_PM3_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_PM4:
        {
            PM_ReleaseConstraints(powerMode, APP_PM4_CONSTRAINTS);
            break;
        }
        default:
        {
            /* PM0/PM1 has no reousrce constraints. */
            PM_ReleaseConstraints(powerMode, 0U);
            break;
        }
    }
}

/*${function:end}*/
