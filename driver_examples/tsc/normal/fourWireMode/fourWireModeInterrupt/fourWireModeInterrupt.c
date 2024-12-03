/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_tsc.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_tscTouch = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief TSC IRQ handler.
 */
void EXAMPLE_TSC_IRQHandler(void)
{
    if ((TSC_GetInterruptStatusFlags(DEMO_TSC_BASE) & kTSC_DetectSignalFlag) == kTSC_DetectSignalFlag)
    {
        TSC_ClearInterruptStatusFlags(DEMO_TSC_BASE, kTSC_DetectSignalFlag);
        TSC_StartMeasure(DEMO_TSC_BASE);
    }
    else
    {
        if ((TSC_GetInterruptStatusFlags(DEMO_TSC_BASE) & kTSC_ValidSignalFlag) == kTSC_ValidSignalFlag)
        {
            TSC_ClearInterruptStatusFlags(DEMO_TSC_BASE, kTSC_ValidSignalFlag);
            g_tscTouch = true;
        }
        TSC_ClearInterruptStatusFlags(DEMO_TSC_BASE, kTSC_MeasureSignalFlag);
    }
    SDK_ISR_EXIT_BARRIER;
}

/*
 *
 * The example indicate how to make TSC, ADC  and touchscreen work together in interrupt mode.
 * Once touchscreen is touched, serial terminal will print the information about x/y-coordinate.
 */
int main(void)
{
    tsc_config_t k_tscConfig;

    BOARD_InitHardware();

    PRINTF("TSC fourWireModeInterrupt Example Start!\r\n");

    TSC_GetDefaultConfig(&k_tscConfig);
    TSC_Init(DEMO_TSC_BASE, &k_tscConfig);
    /* Please enable interrupts and corresponding interrupt signals to insure proper interrupt operation.  */
    TSC_EnableInterruptSignals(DEMO_TSC_BASE,
                               kTSC_ValidSignalEnable | kTSC_MeasureSignalEnable | kTSC_DetectSignalEnable);
    TSC_EnableInterrupts(DEMO_TSC_BASE, kTSC_MeasureInterruptEnable | kTSC_DetectInterruptEnable);
    PRINTF("Please touch screen.\r\n");

    while (1)
    {
        TSC_SoftwareReset(DEMO_TSC_BASE);
        TSC_StartSenseDetection(DEMO_TSC_BASE);
        while ((TSC_GetStatusFlags(DEMO_TSC_BASE) & kTSC_StateMachineFlag) != kTSC_IdleState)
        {
        }
        if (g_tscTouch)
        {
            g_tscTouch = false;
            PRINTF("x = %d, y = %d\r\n", TSC_GetMeasureValue(DEMO_TSC_BASE, kTSC_XCoordinateValueSelection),
                   TSC_GetMeasureValue(DEMO_TSC_BASE, kTSC_YCoordinateValueSelection));
        }
    }
}
