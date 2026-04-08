/*
 * Copyright 2017, 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_pwt.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool busyWait;
volatile bool overflowFlag;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief ISR for PWT interrupt
 *
 * This function changes the state of busyWait.
 */
void PWT0_READY_IRQHandler(void)
{
    if (PWT_GetStatusFlags(EXAMPLE_PWT_BASE) & kPWT_PulseWidthValidFlag)
    {
        /*
         * Disable PWT pulse ready interrupt, ;
         * we do not want to clear the PWTRDY status bit before reading the data
         */
        PWT_DisableInterrupts(EXAMPLE_PWT_BASE, kPWT_PulseWidthReadyInterruptEnable);
        busyWait = false;
    }
    SDK_ISR_EXIT_BARRIER;
}
void PWT0_OVERFLOW_IRQHandler(void)
{
    if (PWT_GetStatusFlags(EXAMPLE_PWT_BASE) & kPWT_CounterOverflowFlag)
    {
        /* Clear overflow flag */
        PWT_ClearStatusFlags(EXAMPLE_PWT_BASE, kPWT_CounterOverflowFlag);
        overflowFlag = true;
    }
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    pwt_config_t pwtConfig;
    uint16_t pulseWidth = 0;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\nPWT example: input signal whose pulse width needs to be measured\r\n");

    /* Init PWT */
    PWT_GetDefaultConfig(&pwtConfig);
    PWT_Init(EXAMPLE_PWT_BASE, &pwtConfig);

    /* Enable at the NVIC */
    EnableIRQ(PWT0_OVERFLOW_IRQn);
    EnableIRQ(PWT0_READY_IRQn);

    /* Enable Module Interrupt  */
    PWT_EnableInterrupts(EXAMPLE_PWT_BASE, kPWT_ModuleInterruptEnable);
    /* Start the PWT counter */
    PWT_StartTimer(EXAMPLE_PWT_BASE);

    uint32_t t = 0U;

    /* This loop will set the print the pulse width */
    while (1)
    {
        busyWait     = true;
        overflowFlag = false;

        /* Enable PWT pulse ready interrupt */
        PWT_EnableInterrupts(EXAMPLE_PWT_BASE,
                             kPWT_PulseWidthReadyInterruptEnable | kPWT_CounterOverflowInterruptEnable);

        /* Wait till ready interrupt occurs */
        while (busyWait)
        {
        }
        if (overflowFlag)
        {
            PRINTF("\r\nPWT overflow occurs\r\n");
        }
        else
        {
            pulseWidth = PWT_ReadPositivePulseWidth(EXAMPLE_PWT_BASE);
            pulseWidth = ((pulseWidth >> 8U) & 0xffU) | ((pulseWidth << 8U) & 0xff00U);
            t          = COUNT_TO_USEC(pulseWidth, PWT_SOURCE_CLOCK);
            PRINTF("\r\nPositive pulse width=%d usec\r\n", t);

            pulseWidth = PWT_ReadNegativePulseWidth(EXAMPLE_PWT_BASE);
            pulseWidth = ((pulseWidth >> 8U) & 0xffU) | ((pulseWidth << 8U) & 0xff00U);
            t          = COUNT_TO_USEC(pulseWidth, PWT_SOURCE_CLOCK);
            PRINTF("\r\nNegative pulse width=%d usec\r\n", t);
        }
        /* Clear pulse ready flag */
        PWT_ClearStatusFlags(EXAMPLE_PWT_BASE, kPWT_PulseWidthValidFlag);
    }
}
