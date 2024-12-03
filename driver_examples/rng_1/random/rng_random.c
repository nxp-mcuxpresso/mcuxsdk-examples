/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "app.h"
#include "fsl_rng.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define RNG_EXAMPLE_RANDOM_NUMBERS     (4)
#define RNG_EXAMPLE_RANDOM_BYTES       (16)
#define RNG_EXAMPLE_RANDOM_NUMBER_BITS (RNG_EXAMPLE_RANDOM_NUMBERS * 8 * sizeof(uint32_t))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief RNG example.
 * @details Generate random numbers and print them to terminal.
 */

int main(void)
{
    uint32_t number;
    uint32_t data[RNG_EXAMPLE_RANDOM_NUMBERS];

    status_t status = kStatus_Fail;

    /* Init hardware*/
    BOARD_InitHardware();

    PRINTF("RNG Peripheral Driver Example\r\n\r\n");

    RNG_Init(RNG);

    while (1)
    {
        PRINTF("Generate %u-bit random number: \r\n", RNG_EXAMPLE_RANDOM_NUMBER_BITS);

        /* Get Random data */

        status = RNG_GetRandomData(RNG, &data, RNG_EXAMPLE_RANDOM_BYTES);

        if (status != kStatus_Success)
        {
            return status;
        }

        /* Print data */
        PRINTF("0x");
        for (number = 0; number < RNG_EXAMPLE_RANDOM_NUMBERS; number++)
        {
            PRINTF("%08X", data[number]);
        }
        PRINTF("\r\n");

        /* Print a note */
        PRINTF(" Press any key to continue... \r\n\r\n");
        GETCHAR();
    }
}
