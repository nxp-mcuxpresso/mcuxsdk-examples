/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_rng.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_NUM_BYTES (16)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t rand_data_buf[MAX_NUM_BYTES];

/*******************************************************************************
 * Code
 ******************************************************************************/
void rng_updateonce_example(void)
{
    trng_config_t user_config;
    uint32_t rand_data;

    TRNG_GetDefaultConfig(&user_config);

    /* Change to update once mode */
    user_config.mode = trng_UpdateOnce;
    TRNG_Init(RNG, &user_config);

    /* Read 32bit data */
    TRNG_GetRandomData(RNG, &rand_data, sizeof(uint32_t));

    PRINTF("Update once example: 0x%x \r\n", rand_data);

    return;
}

void rng_freerunning_example(void)
{
    trng_config_t user_config;
    uint32_t rand_data;
    uint32_t i;
    uint32_t j;

    TRNG_GetDefaultConfig(&user_config);

    /* Initialise RNG */
    TRNG_Init(RNG, &user_config);

    /* Read various data sizes */
    for (i = 1; i <= MAX_NUM_BYTES; i++)
    {
        PRINTF("No. of bytes Read: 0x%x\r\n", i);

        /* Reset buffer to known value */
        for (j = 0; j < i; j++)
        {
            rand_data_buf[j] = 0;
        }

        /* Read random data */
        TRNG_GetRandomData(RNG, &rand_data_buf[0], (i * sizeof(uint8_t)));

        /* Print random data */
        for (j = 0; j < i; j++)
        {
            PRINTF("Byte: 0x%x Random data: 0x%x \r\n", j, rand_data_buf[j]);
        }
    }

    /* Read random data in a loop */
    while (1)
    {
        TRNG_GetRandomData(RNG, &rand_data, sizeof(uint32_t));

        PRINTF("Free running example: 0x%x\r\n", rand_data);
    }

    return;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init the boards */
    BOARD_InitHardware();

    PRINTF("RNG Examples \n\r");

    /* Update once example */
    rng_updateonce_example();

    /* FreeRunning example */
    rng_freerunning_example();

    /* Should never arrive here */
    return 1;
}
