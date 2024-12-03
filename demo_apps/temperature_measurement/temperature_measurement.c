/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("\r\n Temperature measurement example.");
    PRINTF("\r\n Please press any key to get the temperature.");
    while (1)
    {
        GETCHAR();
        PRINTF("\r\n Current temperature is: %.3f degrees Celsius.", ((double)DEMO_MeasureTemperature()));
    }
}
