/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_rit.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void APP_RIT_HANDLER(void)
{
    RIT_ClearStatusFlags(RIT, kRIT_TimerFlag);
    LED_TOGGLE();
    SDK_ISR_EXIT_BARRIER;
}
/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize RIT */
    rit_config_t ritConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Init LED. */
    LED_INIT();

    /*
     * ritConfig.enableRunInDebug = false;
     */
    RIT_GetDefaultConfig(&ritConfig);

    /* Init rit module */
    RIT_Init(RIT, &ritConfig);

    /* Set timer period for Compare register. */
    RIT_SetTimerCompare(RIT, RIT_SOURCE_CLOCK);

    /* Set to enable the Counter register auto clear to zero when the counter value equals the set period. */
    RIT_SetCountAutoClear(RIT, true);

    PRINTF("RIT Example Start, You will see spcified LED blink at 1s period interval.\r\n");

    /* Start counting */
    RIT_StartTimer(RIT);

    /* Enable at the NVIC */
    EnableIRQ(RIT_IRQ_ID);

    while (1)
    {
    }
}
