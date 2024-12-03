/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_ccm32k.h"
#include "fsl_rtc.h"
/*${header:end}*/

extern volatile uint8_t g_AlarmPending;
extern volatile bool g_SecsFlag;

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    ccm32k_osc_config_t osc32kConfig = {
        .enableInternalCapBank = true,
        .xtalCap               = kCCM32K_OscXtal0pFCap,
        .extalCap              = kCCM32K_OscExtal16pFCap,
        .coarseAdjustment      = kCCM32K_OscCoarseAdjustmentRange0,
    };
    CCM32K_Set32kOscConfig(CCM32K, kCCM32K_Enable32kHzCrystalOsc, &osc32kConfig);
    CCM32K_SelectClockSource(CCM32K, kCCM32K_ClockSourceSelectOsc32k);
}

/*!
 * @brief Override the RTC IRQ handler.
 */
void RTC_Alarm_IRQHandler(void)
{
    if (RTC_GetStatusFlags(RTC) & kRTC_AlarmFlag)
    {
        g_AlarmPending = 1U;

        /* Clear alarm flag */
        RTC_ClearStatusFlags(RTC, kRTC_AlarmInterruptEnable);
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    exception return operation might vector to incorrect interrupt */
    __DSB();
}

/*!
 * @brief Override the RTC Second IRQ handler.
 */
void RTC_Seconds_IRQHandler(void)
{
    g_SecsFlag = true;
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    exception return operation might vector to incorrect interrupt */
    __DSB();
}

/*${function:end}*/
