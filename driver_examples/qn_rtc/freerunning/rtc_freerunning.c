/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "fsl_rtc.h"
#include "clock_config.h"

#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RTC_INTERRUPT_THRESHOLD (CLOCK_GetFreq(kCLOCK_32KClk))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t g_RtcFreeRunningFlag;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware. */
    BOARD_InitHardware();
    PRINTF("\r\nRTC free running example.\r\n");

    RTC_Init(RTC);

    /* Enable RTC free running interrupt */
    RTC_EnableInterrupts(RTC, kRTC_FreeRunningInterruptEnable);
    /* Enable at the NVIC */
    NVIC_EnableIRQ(RTC_FR_IRQn);

    RTC_SetFreeRunningInterruptThreshold(RTC, RTC_INTERRUPT_THRESHOLD);
    RTC_FreeRunningEnable(RTC, true);

    uint32_t cnt         = 0;
    g_RtcFreeRunningFlag = 0;
    while (1)
    {
        if (g_RtcFreeRunningFlag)
        {
            g_RtcFreeRunningFlag = 0;
            PRINTF("RTC Free running interrupt occur count = %d\r\n", cnt++);
        }
    }
}

void RTC_FR_IRQHandler(void)
{
    if (RTC_GetStatusFlags(RTC) & (uint32_t)kRTC_FreeRunningInterruptFlag)
    {
        g_RtcFreeRunningFlag = 1;
        RTC_SetFreeRunningInterruptThreshold(RTC, RTC_GetFreeRunningInterruptThreshold(RTC) + RTC_INTERRUPT_THRESHOLD);
    }
    SDK_ISR_EXIT_BARRIER;
}
