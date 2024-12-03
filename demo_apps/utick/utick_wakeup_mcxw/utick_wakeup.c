/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_common.h"
#include "fsl_power.h"
#include "fsl_utick.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define UTICK_TIME 3000000
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
void delay(void);

void BOARD_BootToIrc()
{
    APP_INTERNAL_IRC();
}

/*******************************************************************************
 * Code
 ******************************************************************************/
void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 100000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    /* Running core to internal clock 12 MHz*/
    BOARD_BootToIrc();

    /* Init output LED GPIO. */
    APP_LED_INIT;

    PRINTF("Utick wakeup demo start...\r\n");

    /* Attach Main Clock as CLKOUT */
    CLOCK_AttachClk(kMAIN_CLK_to_CLKOUT);

    /* Set the clock dividor to divide by 2*/
    CLOCK_SetClkDiv(kCLOCK_DivClkOut, 2, false);

    /* Intiialize UTICK */
    UTICK_Init(UTICK0);

    /* Set the UTICK timer to wake up the device from reduced power mode */
    UTICK_SetTick(UTICK0, kUTICK_Repeat, UTICK_TIME, NULL);

#if defined(APP_UTICK_WAKEUP_FROM_SLEEP_MODE) && APP_UTICK_WAKEUP_FROM_SLEEP_MODE
    /* Enter sleep mode. */
    POWER_EnterSleep();
#else
/* Enter Deep Sleep mode */
#if (defined(FSL_FEATURE_POWERLIB_EXTEND) && FSL_FEATURE_POWERLIB_EXTEND)
    POWER_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP, WAKEUP_UTICK);
#else
    POWER_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);
#endif
#endif

    /* Set the clock dividor to divide by 1*/
    CLOCK_SetClkDiv(kCLOCK_DivClkOut, 1, false);

#if defined(APP_UTICK_WAKEUP_FROM_SLEEP_MODE) && APP_UTICK_WAKEUP_FROM_SLEEP_MODE
    PRINTF("Wakeup from sleep mode...\r\n");
#else
    PRINTF("Wakeup from deep sleep mode...\r\n");
#endif

    while (1)
    {
        /* Toggle LED */
        APP_LED_TOGGLE;
        delay();
    }
}
