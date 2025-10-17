/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_swt.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_swtRefreshCounter = 0U;
volatile uint32_t g_systickCounter;
swt_service_mode_t g_serviceMode = kSWT_FixedServiceSequence;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief SWT IRQ handler
 *
 * This function will clear SWT interrupt flag and refresh SWT.
 */
void SWT_HANDLER(void)
{
    PRINTF("SWT timeout interrupt triggered %d!\r\n", g_swtRefreshCounter++);
    SWT_ClearTimeoutInterruptFlag(SWT_BASE);
    if (g_serviceMode == kSWT_FixedServiceSequence)
    {
        SWT_RefreshWithFixedServiceSequence(SWT_BASE);
    }
    else
    {
        SWT_RefreshWithKeyedServiceSequence(SWT_BASE);
    }
}

/*!
 * @brief SysTick IRQ handler
 *
 */
void SysTick_Handler(void)
{
    g_systickCounter--;
}

/*!
 * @brief Swt regular mode
 *
 * This function will feed SWT within fixed service sequence and keyed service sequence 3 times under regular mode.
 *
 */
void SWT_RegularMode(void)
{
    PRINTF("\r\n");
    swt_config_t config;

    SWT_GetDefaultConfig(&config);
    config.timeoutValue = SWT_TIMEOUT;
    config.interruptThenReset = true;
    config.enableWindowMode = false;
    PRINTF("---     SWT regular(non window) mode      ---\r\n");
    PRINTF("Refresh SWT with fixed service sequence:\r\n");
    EnableIRQ(SWT_IRQ_ID);
    SWT_Init(SWT_BASE, &config);
    for(;;)
    {
        if(g_swtRefreshCounter == SWT_REFRESH_COUNT)
        {
            g_swtRefreshCounter = 0U;
            SWT_Disable(SWT_BASE);
            break;
        }
    }

    PRINTF("\r\nRefresh SWT with keyed service sequence:\r\n");
    g_serviceMode = kSWT_KeyedServiceSequence;
    config.serviceMode = kSWT_KeyedServiceSequence;
    config.serviceKey = SWT_SERVICE_KEY;
    SWT_Init(SWT_BASE, &config);
    for(;;)
    {
        if(g_swtRefreshCounter == SWT_REFRESH_COUNT)
        {
            g_swtRefreshCounter = 0U;
            SWT_Disable(SWT_BASE);
            break;
        }
    }
}

/*!
 * @brief Swt window mode
 *
 * This function will feed SWT within fixed service sequence under window mode.
 *
 */
void SWT_WindowMode(void)
{
    PRINTF("\r\n");
    swt_config_t config;

    SWT_GetDefaultConfig(&config);
    config.timeoutValue = SWT_TIMEOUT;
    config.enableWindowMode = true;
    /* SWT can be refreshed in the second half of SWT_TIMEOUT, so app can refresh SWT during 0.5-1s after SWT_Init() */
    config.windowValue = SWT_TIMEOUT / 2U;
    /* Systick count down to 0 after 0.7s */
    g_systickCounter = SWT_WINDOW_OPERATE_TIME;

    PRINTF("---            SWT window mode            ---\r\n");
    SWT_Init(SWT_BASE, &config);
    // Wait for window
    API_SysTick_Start();
    while(g_systickCounter > 0)
    {
    }

    SWT_RefreshWithFixedServiceSequence(SWT_BASE);
    PRINTF("SWT refresh with fixed service sequence in window mode done!\r\n");
    SWT_Disable(SWT_BASE);
}

int main(void)
{
    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    PRINTF("=== Software watchdog timer example start ===\r\n");
    SWT_RegularMode();
    SWT_WindowMode();
    PRINTF("===  Software watchdog timer example end  ===\r\n");
    while(1)
    {
    }
}
