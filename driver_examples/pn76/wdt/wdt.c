/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_wdt.h"
#include "fsl_gpt.h"
#include "board.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool g_wdtFlag = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
/******** pn76 wdt handler ***********/
void NMI_Handler(void)
{
    WDT_StopTimer(TIMERS);
    WDT_ClearTimeoutStatus(TIMERS);
    g_wdtFlag = true;
}

void Delay(uint32_t count_ms)
{
    uint32_t count = (SystemCoreClock / 1000) * count_ms;
    while (count--)
    {
        __NOP();
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t count = 10;

    gpt_timer_config_t timerConfig = {
        .prescalerFactor = DEMO_WDT_PRE_COUNT,
    };
    /* Init board hardware. */
    BOARD_InitHardware();
    PRINTF("\r\nSTART DEMO WDT\r\n");

    GPT_Init(TIMERS, &timerConfig);
    /* Config the WDT trigger value*/
    WDT_SetInterruptThreshold(TIMERS, DEMO_WDT_TRIGGER_COUNT);
    /* Config the WDT timeout value and prescaler value, start WDT*/
    WDT_StartTimer(TIMERS, DEMO_WDT_TIMEOUT_COUNT);

    /* Feed the WDOG 10 times. */
    while (count--)
    {
        Delay(10);
        WDT_Feed(TIMERS);
        PRINTF("\r\nWDT_FEED\r\n");
    }

    /* Wait watch dog interrupt happens. */
    while (!g_wdtFlag)
    {
    }

    PRINTF("\r\nDEMO WDT FINISHED\r\n");

    while (1)
    {
    }
}
