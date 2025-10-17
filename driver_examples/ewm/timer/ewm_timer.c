/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_ewm.h"
#include "fsl_adapter_timer.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
bool is_timer_timeout(void);
void timer_clr_event(void);
#define ACTION "Timeout"
#define TIMER_NAME "timer"
#if !defined(EXAMPLE_TIMEOUT_PERIOD_COUNT) || !defined(EXAMPLE_TIMER_CLK_FREQ) || !defined(EXAMPLE_TIMER_INSTANCE)
#error "Pls define EXAMPLE_TIMEOUT_PERIOD_COUNT, EXAMPLE_TIMER_CLK_FREQ, EXAMPLE_TIMER_INSTANCE in app.h to use timer event"
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool ewmIsrFlag = false;
volatile bool timeoutFlag                      = false;
hal_timer_handle_t g_timerHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief EWM interrupt handler
 *
 */
void WDOG_EWM_IRQHandler(void)
{
    EWM_DisableInterrupts(EXAMPLE_EWM, kEWM_InterruptEnable); /*!< de-assert interrupt request */
    ewmIsrFlag = true;
    SDK_ISR_EXIT_BARRIER;
}

/* Timer call back. */
void TIMER_UserCallback(void *param)
{
    timeoutFlag = true;
    HAL_TimerDisable(&g_timerHandle);
}

bool is_timer_timeout(void)
{
    return timeoutFlag;
}

void timer_clr_event(void)
{
    timeoutFlag = false;
}

static void EXAMPLE_InitTimer(void)
{
    hal_timer_config_t timerConfig;

    timeoutFlag             = false;
    timerConfig.timeout     = EXAMPLE_TIMEOUT_PERIOD_COUNT;
    timerConfig.srcClock_Hz = EXAMPLE_TIMER_CLK_FREQ;
    timerConfig.instance    = EXAMPLE_TIMER_INSTANCE;

    (void)memset(&g_timerHandle, 0, sizeof(g_timerHandle));

    /* Initialize the timer. */
    HAL_TimerInit(&g_timerHandle, &timerConfig);
    /* Install call back function. */
    HAL_TimerInstallCallback(&g_timerHandle, TIMER_UserCallback, NULL);
}

/*!
 * @brief Main function
 */
int main(void)
{
    ewm_config_t config;

    /* Init hardware*/
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    EXAMPLE_InitTimer();

    /* EWM peripheral driver test */
    /*
     * config.enableEwm = true;
     * config.enableEwmInput = false;
     * config.setInputAssertLogic = false;
     * config.enableInterrupt = false;
     * config.clockSource = kEWM_LpoClockSource0;
     * config.prescaler = 0U;
     * config.compareLowValue = 0U;
     * config.compareHighValue = 0xFEU;
     */
    EWM_GetDefaultConfig(&config);
    config.enableInterrupt = true;
    NVIC_EnableIRQ(WDOG_EWM_IRQ);
    EWM_Init(EXAMPLE_EWM, &config);

    PRINTF("\r\n EWM example ");
    PRINTF("\r\n %s %s to expire EWM ", TIMER_NAME, ACTION);
    while (1)
    {
        /* Restart counter*/
        EWM_Refresh(EXAMPLE_EWM);

        /* start timer */
        HAL_TimerEnable(&g_timerHandle);
        /* Check event flag*/
        if (is_timer_timeout())
        {
            timer_clr_event();
            /*Check ewm interrupt occur*/
            while (!ewmIsrFlag)
            {
            }
            PRINTF("\r\n EWM interrupt is occurred");
            PRINTF("\r\n %s %s to expire EWM again", TIMER_NAME, ACTION);

            /* Clear interrupt flag*/
            ewmIsrFlag = false;
            /*Restart counter and enable interrupt for next run*/
            EWM_Refresh(EXAMPLE_EWM);
            /*Enable EWM interrupt*/
            EWM_EnableInterrupts(EXAMPLE_EWM, kEWM_InterruptEnable);
            /* start timer */
            HAL_TimerEnable(&g_timerHandle);
        }
    }
}
