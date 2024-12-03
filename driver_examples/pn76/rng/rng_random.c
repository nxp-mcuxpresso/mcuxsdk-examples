/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_rng.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Amount of the members in data */
#define RNG_EXAMPLE_DATA_NUMBERS (3U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t get_and_print(uint8_t *data_temp, char *data_name, status_t (*func)(uint8_t *, uint32_t))
{
    uint32_t number = 0U;
    status_t ret    = kStatus_InvalidArgument;

    /* Get data */
    ret = func(data_temp, RNG_EXAMPLE_DATA_NUMBERS);

    if (ret != kStatus_Success)
    {
        return ret;
    }

    /* Print data */
    for (number = 0; number < RNG_EXAMPLE_DATA_NUMBERS; number++)
    {
        PRINTF("%s[%d]: 0x%02X  ", data_name, number, data_temp[number]);
    }
    PRINTF("\r\n");
    return ret;
}

/*!
 * @brief Main function.
 */
int main(void)
{
    /* Init data. */
    uint8_t random_data[RNG_EXAMPLE_DATA_NUMBERS] = {0};
    uint8_t secure_data[RNG_EXAMPLE_DATA_NUMBERS] = {0};
    status_t status                               = kStatus_InvalidArgument;

    /* Init hardware. */
    BOARD_InitHardware();
    RNG_Init();

    while (1)
    {
        /* Get and print random data. */
        status = get_and_print(random_data, "random_data", RNG_GenerateRng);
        if (status != kStatus_Success)
        {
            PRINTF("Invalid argument! Get random data error!\r\n");
        }

        /* Get and print secure data. */
        status = get_and_print(secure_data, "secure_data", RNG_GenerateSecureRng);
        if (status != kStatus_Success)
        {
            PRINTF("Invalid argument! Get secure data error!\r\n");
        }

        /* Print a note. */
        PRINTF("Press any key to continue... \r\n\r\n");
        GETCHAR();
    }
}
