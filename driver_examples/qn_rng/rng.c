/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_rng.h"
#include "fsl_power.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RNG_EXAMPLE_RANDOM_NUMBER 10

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
    uint32_t i;
    status_t status;
    uint8_t data[RNG_EXAMPLE_RANDOM_NUMBER];

    /* Initialize board hardware. */
    BOARD_InitHardware();
    PRINTF("\r\nRandom number generator example.\r\n");

    /* Enable ADC power */
    POWER_EnableADC(true);

    /* Configure the RNG */
    RNG_Init(DEMO_RNG_BASE);
    RNG_Enable(DEMO_RNG_BASE, true);
    PRINTF("RNG configuration Done.\r\n");
    PRINTF("Input any character to get random number ...\r\n");

    while (1)
    {
        PRINTF("Generate %d random numbers: \r\n", RNG_EXAMPLE_RANDOM_NUMBER);

        /* Get Random data*/
        status = RNG_GetRandomData(RNG, data, sizeof(data));
        if (status == kStatus_Success)
        {
            /* Print data*/
            for (i = 0; i < RNG_EXAMPLE_RANDOM_NUMBER; i++)
            {
                PRINTF("Random[%d] = 0x%X\r\n", i, data[i]);
            }
        }
        else
        {
            PRINTF("TRNG failed!\r\n");
        }

        /* Print a note.*/
        PRINTF("\r\nInput any character to continue... \r\n");
        GETCHAR();
    }
}
