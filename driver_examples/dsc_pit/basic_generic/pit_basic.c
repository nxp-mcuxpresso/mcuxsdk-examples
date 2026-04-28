/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_pit.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef PIT_IRQ_PRIO
#define PIT_IRQ_PRIO 0
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool pitIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void PIT_LED_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(DEMO_PIT_BASEADDR);
    pitIsrFlag = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    pit_config_t pitConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    /* Initialize and enable LED */
    LED_INIT();

    /*
     *    psConfig->ePrescaler = kPIT_PrescalerDivBy1;
     *    psConfig->bEnableInterrupt = false;
     *    psConfig->bEnableSlaveMode = false;
     *    psConfig->bEnableTimer = false;
     *    psConfig->eClockSource = kPIT_CountClockSource0;
     *    psConfig->u16PeriodCount = 0xFFFFU;
     */
    PIT_GetDefaultConfig(&pitConfig);
    pitConfig.eClockSource = PIT_CLOCK_SOURCE;
    pitConfig.ePrescaler   = PIT_CLOCK_SOURCE_DIVIDER;
#if defined(FSL_FEATURE_PIT_32BIT_COUNTER) && FSL_FEATURE_PIT_32BIT_COUNTER
    pitConfig.u32PeriodCount = PIT_PERIOD_COUNT;
#else
    pitConfig.u16PeriodCount = PIT_PERIOD_COUNT;
#endif
    pitConfig.bEnableInterrupt = true;
    pitConfig.bEnableTimer     = true;

    /* Initialize pit module */
    PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);

    /* Enable IRQ with priority */
    EnableIRQWithPriority(PIT_IRQ_ID, PIT_IRQ_PRIO);

    /* Start PIT timer */
    PRINTF("\r\nStarting PIT timer ...");
    PIT_StartTimer(DEMO_PIT_BASEADDR);

    while (true)
    {
        /* Check whether occur interrupt and toggle LED */
        if (true == pitIsrFlag)
        {
            PRINTF("\r\n PIT timer interrupt is occurred!");
            LED_TOGGLE();
            pitIsrFlag = false;
        }
    }
}
