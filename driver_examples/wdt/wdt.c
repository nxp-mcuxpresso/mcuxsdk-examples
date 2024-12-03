/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_wdt.h"
#include "fsl_reset.h"
#include "fsl_gpio.h"

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
void EXAMPLE_WDT_IRQHandler(void)
{
    /* clear the interrupt status */
    WDT_ClearStatusFlags(EXAMPLE_WDT, kWDT_TimeoutFlag);
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
    wdt_config_t config;

    BOARD_InitHardware();

    WDT_GetDefaultConfig(&config);

    config.timeoutCounter = EXAMPLE_WDT_COUNT;
    config.enableReset    = false;
    /* Config timeout value = 4*(EXAMPLE_WDT_COUNT+1)*EXAMPLE_WDT_CLKDIV/SFRO_FREQUENCY */
    WDT_Init(EXAMPLE_WDT, &config);
    EnableIRQ(EXAMPLE_WDT_IRQ);

    /* Feed the WDOG 10 times. */
    uint32_t count = 10;
    while (count--)
    {
        Delay(10);
        WDT_Feed(EXAMPLE_WDT);
    }

    /* Wait watch dog interrupt happens. */
    while (!g_wdtFlag)
    {
        __NOP();
    }

    /* Display diget 0 if watch interrupt happens. */
    BOARD_LedMatrixShowNumber(0, 0, true);

    while (1)
    {
    }
}
