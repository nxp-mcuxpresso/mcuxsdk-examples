/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2022, 2026 NXP
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
#ifndef EXAMPLE_PWT_BASE
#define EXAMPLE_PWT_BASE PWT
#endif

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
void PWT_EXAMPLE_HANDLER(void)
{
    uint32_t status = PWT_GetStatusFlags(EXAMPLE_PWT_BASE);

    if (status & kPWT_PulseWidthValidFlag)
    {
        /*
         * Disable PWT pulse ready interrupt, ;
         * we do not want to clear the PWTRDY status bit before reading the data
         */
        PWT_DisableInterrupts(EXAMPLE_PWT_BASE, kPWT_PulseWidthReadyInterruptEnable);
        busyWait = false;
    }

    if (status & kPWT_CounterOverflowFlag)
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

    /* Init PWT */
    PWT_GetDefaultConfig(&pwtConfig);
    pwtConfig.inputSelect = kPWT_InputPort_1;
    PWT_Init(EXAMPLE_PWT_BASE, &pwtConfig);

    PRINTF("\r\nPWT example: input signal whose pulse width needs to be measured\r\n");

    /* Enable at the NVIC */
    EnableIRQ(PWT_INTERRUPT_NUMBER);

    /* Start the PWT counter */
    PWT_StartTimer(EXAMPLE_PWT_BASE);

    /* This loop will set the print the pulse width */
    while (1)
    {
        busyWait     = true;
        overflowFlag = false;

        /* Enable PWT pulse ready interrupt */
        PWT_EnableInterrupts(EXAMPLE_PWT_BASE, kPWT_PulseWidthReadyInterruptEnable);

        /* Wait till ready interrupt occurs */
        while (busyWait)
        {
        }

        if (overflowFlag)
        {
            uint8_t level     = PWT_GetInputLevel(EXAMPLE_PWT_BASE);
            bool toggled      = (PWT_GetStatusFlags(EXAMPLE_PWT_BASE) & kPWT_InputToggleFlag) != 0U;

            if (toggled)
            {
                /* Clear the toggle flag */
                PWT_ClearStatusFlags(EXAMPLE_PWT_BASE, kPWT_InputToggleFlag);

                if (toggled && (level == 0U))
                {
                    PRINTF("\r\nToggled Low overflow\r\n");
                }
                else
                {
                    PRINTF("\r\nToggled High overflow\r\n");
                }
            }
            else
            {
                if (level == 0U)
                {
                    PRINTF("\r\nLow overflow (0 duty ratio), signal stayed low\r\n");
                }
                else
                {
                    PRINTF("\r\nHigh overflow (100%% duty ratio), signal stayed high\r\n");
                }
            }
        }
        else
        {
            pulseWidth = PWT_ReadPositivePulseWidth(EXAMPLE_PWT_BASE);
            pulseWidth = COUNT_TO_USEC(pulseWidth, PWT_SOURCE_CLOCK);
            PRINTF("\r\nPositive pulse width=%d usec\r\n", pulseWidth);

            pulseWidth = PWT_ReadNegativePulseWidth(EXAMPLE_PWT_BASE);
            pulseWidth = COUNT_TO_USEC(pulseWidth, PWT_SOURCE_CLOCK);
            PRINTF("\r\nNegative pulse width=%d usec\r\n", pulseWidth);
        }
        /* Clear pulse ready flag */
        PWT_ClearStatusFlags(EXAMPLE_PWT_BASE, kPWT_PulseWidthValidFlag);
    }
}
