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

/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * The example indicate how to make TSC, ADC  and touchscreen work together in polling(auto-measure) mode.
 * Once touchscreen is touched, serial terminal will print the information about x/y-coordinate.
 */
int main(void)
{
    tsc_config_t k_tscConfig;

    BOARD_InitHardware();

    PRINTF("TSC fourWireModePolling Example Start!\r\n");

    TSC_GetDefaultConfig(&k_tscConfig);
    k_tscConfig.enableAutoMeasure = true;
    TSC_Init(DEMO_TSC_BASE, &k_tscConfig);
    TSC_EnableInterruptSignals(DEMO_TSC_BASE, kTSC_ValidSignalEnable);
    PRINTF("Please touch screen.\r\n");

    while (1)
    {
        TSC_SoftwareReset(DEMO_TSC_BASE);
        TSC_StartSenseDetection(DEMO_TSC_BASE);
        while ((TSC_GetStatusFlags(DEMO_TSC_BASE) & kTSC_StateMachineFlag) != kTSC_IdleState)
        {
        }
        if ((TSC_GetInterruptStatusFlags(DEMO_TSC_BASE) & kTSC_ValidSignalFlag) == kTSC_ValidSignalFlag)
        {
            TSC_ClearInterruptStatusFlags(DEMO_TSC_BASE, kTSC_ValidSignalFlag);
            PRINTF("x = %d, y = %d\r\n", TSC_GetMeasureValue(TSC, kTSC_XCoordinateValueSelection),
                   TSC_GetMeasureValue(TSC, kTSC_YCoordinateValueSelection));
        }
    }
}
