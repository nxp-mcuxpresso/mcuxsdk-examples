/*
 * Copyright 2025 NXP
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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool g_pitIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void PIT_LED_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearRtiStatusFlags(DEMO_PIT_BASEADDR, kPIT_RtiTimerFlag);
    g_pitIsrFlag = true;
    /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
     * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
     * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
     */
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Initialize and enable LED */
    LED_INIT();

    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit rti timer */
    PIT_RTI_Init(DEMO_PIT_BASEADDR, &pitConfig);

    /* Set timer period for RTI */
    PIT_SetRtiTimerPeriod(DEMO_PIT_BASEADDR, USEC_TO_COUNT(1000000U, PIT_SOURCE_CLOCK));

    /* Enable timer interrupts for RTI */
    PIT_EnableRtiInterrupts(DEMO_PIT_BASEADDR, kPIT_RtiTimerInterruptEnable);

    for (uint32_t i = 0; i < 1000U; ++i) /* Delay a little times to wait the value sync to RTI clock domain */
    {
        __ASM("nop");
    }

    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQ_ID);

    /* Start RTI timer */
    PRINTF("\r\nStarting RTI ...");
    PIT_StartRtiTimer(DEMO_PIT_BASEADDR);

    while (true)
    {
        /* Check whether occur interupt and toggle LED */
        if (true == g_pitIsrFlag)
        {
            PRINTF("\r\n RTI interrupt is occurred !");
            LED_TOGGLE();
            g_pitIsrFlag = false;
        }
    }
}
