/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include <stdlib.h>

#include "fsl_debug_console.h"
#include "fsl_wdt.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Default Watch dog clock source is 32K */
#define WDT_CYCLE_NUM 96000U /*About 3000ms*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint8_t wdt_int_flag = 0;
static volatile uint8_t wdt_int_cnt  = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
void WDT_IRQHandler(void)
{
    wdt_int_flag = 1;

    if (wdt_int_cnt < 5)
    {
        WDT_ClearStatusFlags(DEMO_WDT_BASE);
        wdt_int_cnt++;
    }
    else
    {
        /* Wait for watchdog reset */
        while (1)
        {
        }
    }
    SDK_ISR_EXIT_BARRIER;
}
/*!
 * @brief Main function
 */
int main(void)
{
    wdt_config_t config;

    BOARD_InitHardware();
    CLOCK_AttachClk(k32K_to_WDT_CLK);
    PRINTF("\r\nWatchdog example.\r\n");

    /* Check Reset source(First time device power on Watch dog reset source will be set) */
    if (kRESET_SrcWatchDog == RESET_GetResetSource())
    {
        PRINTF("Watchdog reset occurred\r\n");
    }
    /* Clear reset source */
    RESET_ClearResetSource();

    config.enableWdtReset = true;
    config.loadValue      = WDT_CYCLE_NUM;

    WDT_Init(DEMO_WDT_BASE, &config);
    NVIC_EnableIRQ(WDT_IRQn);

    while (1)
    {
        if (wdt_int_cnt < 5)
        {
            PRINTF("Wait for watchdog interrupt\r\n");
        }
        else
        {
            PRINTF(
                "When next watchdog interrupt occurrs and the program will stay in the watchdog interrupt "
                "handler.\r\n");
            PRINTF("WDT will trigger watchdog reset after load_value(3s) * 2\r\n");
        }

        while (wdt_int_flag == 0)
        {
        }
        wdt_int_flag = 0;

        PRINTF("Watchdog interrupt occurred and interrupt is cleared in the watchdog interrupt handler\r\n");
    }
}
