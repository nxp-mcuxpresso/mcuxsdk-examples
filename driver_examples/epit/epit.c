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
#include "fsl_epit.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool epitIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_EPIT_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    EPIT_ClearStatusFlags(EXAMPLE_EPIT, kEPIT_OutputCompareFlag);
    epitIsrFlag = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize EPIT */
    epit_config_t epitConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    EPIT_GetDefaultConfig(&epitConfig);

    /* Init EPIT module */
    EPIT_Init(EXAMPLE_EPIT, &epitConfig);

    /* Set timer period */
    EPIT_SetTimerPeriod(EXAMPLE_EPIT, USEC_TO_COUNT(1000000U, EXAMPLE_EPIT_CLK_FREQ) - 1);
    EPIT_SetOutputCompareValue(EXAMPLE_EPIT, 0);

    /* Enable output compare interrupts */
    EPIT_EnableInterrupts(EXAMPLE_EPIT, kEPIT_OutputCompareInterruptEnable);

    /* Enable at the Interrupt */
    EnableIRQ(EPIT_IRQ_ID);

    /* Start Timer */
    PRINTF("\r\nStarting EPIT timer ...");
    EPIT_StartTimer(EXAMPLE_EPIT);

    while (true)
    {
        /* Check whether occur interupt and toggle LED */
        if (true == epitIsrFlag)
        {
            PRINTF("\r\n EPIT interrupt is occurred !");
            epitIsrFlag = false;
        }
        else
        {
            __WFI();
        }
    }
}
