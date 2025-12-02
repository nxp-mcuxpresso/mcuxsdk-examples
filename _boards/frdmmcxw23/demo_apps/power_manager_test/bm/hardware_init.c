/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"

#include "fsl_pm_core.h"
#include "fsl_pm_device.h"
#include "fsl_rtc.h"
#include "fsl_system.h"

#include "app.h"
/*${header:end}*/

/*${variable:start}*/

extern pm_handle_t g_pmHandle;
extern uint8_t g_targetPowerMode;
/*${variable:end}*/

/*${function:start}*/

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify1) = {
    .notifyCallback = APP_UartControlCallback,
    .data           = NULL,
};

void BOARD_InitHardware(void)
{
    POWER_Init();

    CLOCK_EnableClock(kCLOCK_Gpio0);
    GPIO_PortInit(GPIO, 0);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
    CLOCK_EnableClock(kCLOCK_FlexComm0);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

status_t APP_UartControlCallback(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    static const char *const powerNameArray[APP_TARGET_POWER_NUM] = APP_POWER_NAME;
    if (eventType == kPM_EventEnteringSleep)
    {
        PRINTF("Entering into %s.\r\n", powerNameArray[powerState]);
        if (powerState >= PM_LP_STATE_DPD)
        {
            PRINTF("Note: Wake up from %s casue Normal Boot\r\n", powerNameArray[powerState]);
            if (powerState == PM_LP_STATE_POWER_OFF)
            {
                PRINTF("Please Press RESET Button(SW1) to wakeup from Power Off Mode(RTC is powered off)\r\n");
            }
        }
    }
    else
    {
        PRINTF("Wakeup from %s.\r\n", powerNameArray[powerState]);
    }

    return kStatus_Success;
}

void RTC_IRQHandler(void)
{
    RTC_ClearStatusFlags(RTC, kRTC_WakeupFlag);    
    DisableIRQ(RTC_IRQn);
}


void APP_StartRTC(uint64_t timeoutUs)
{
    RTC_EnableWakeupTimer(RTC, true);
    RTC_SetWakeupCount(RTC, USEC_TO_COUNT(timeoutUs, 1000));
    RTC_EnableTimer(RTC, true);
    EnableIRQ(RTC_IRQn);
}

void APP_StopRTC(void)
{
    RTC_EnableWakeupTimer(RTC, false);
}

void APP_InitWakeupSource(void)
{
    PM_RegisterTimerController(&g_pmHandle, APP_StartRTC, APP_StopRTC, NULL, NULL);
    PRINTF("Calibrating RTC Clock Dividers...\r\n");
    SYSTEM_CalibrateRTCClockDividers();
    
    POWER_PeripheralPowerOn(kPOWERCFG_FRO32K);
    CLOCK_Select32kOscClkSrc(kCLOCK_Osc32kClockSrc_FRO);
    RTC_Init(RTC);
    EnableIRQ(RTC_IRQn);
}

uint32_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;
    uint32_t timeoutUs;

    if (g_targetPowerMode == PM_LP_STATE_POWER_OFF)
    {
        return 0UL;
    }

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
            timeoutUs = timeout * 1000 * 1000;
            return timeoutUs;
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

        case PM_LP_STATE_PD_WITH_CPU_RET:
        {
            PM_SetConstraints(PM_LP_STATE_PD_WITH_CPU_RET, APP_POWER_DOWN_CONSTRAINTS);
            break;
        }

        case PM_LP_STATE_DPD:
        {
            PM_SetConstraints(PM_LP_STATE_DPD, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        
        case PM_LP_STATE_POWER_OFF:
        {
            PM_SetConstraints(PM_LP_STATE_POWER_OFF, APP_POWER_OFF_CONSTRAINTS);
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
        case PM_LP_STATE_PD_WITH_CPU_RET:
        {
            PM_ReleaseConstraints(PM_LP_STATE_PD_WITH_CPU_RET, APP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_DPD:
        {
            PM_ReleaseConstraints(PM_LP_STATE_DPD, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_POWER_OFF:
        {
            PM_ReleaseConstraints(PM_LP_STATE_POWER_OFF, APP_POWER_OFF_CONSTRAINTS);
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
