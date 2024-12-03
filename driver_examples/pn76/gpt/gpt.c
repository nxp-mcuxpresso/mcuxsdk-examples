/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_gpt.h"
#include "board.h"
#include "app.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint8_t s_gpioState = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_GPT_IRQHandler(void)
{
    /* every 1s the gpt callback will be triggered */
    if (GPT_GetTimeoutStatus(TIMERS, kGPT_Timer0))
    {
        GPT_ClearTimeoutStatus(TIMERS, kGPT_Timer0);
        BOARD_WriteGpio(s_gpioState);
        s_gpioState = (s_gpioState == 0U) ? 1U : 0U;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    gpt_timer_config_t timerConfig = {
        .prescalerFactor = DEMO_GPT_TIMER_PRESCALER,
    };
    /* Init board hardware. */
    BOARD_InitHardware();

    GPT_Init(TIMERS, &timerConfig);
    GPT_SetTimerMode(TIMERS, kGPT_Timer0, kGPT_TimerModeSingleFreeRunning);
    GPT_EnableTimeoutInterrupt(TIMERS, kGPT_Timer0, true);

    GPT_StartTimer(TIMERS, kGPT_Timer0, DEMO_GPT_TIMER_TIMEOUT_COUNT);

    while (1)
    {
    }
}
