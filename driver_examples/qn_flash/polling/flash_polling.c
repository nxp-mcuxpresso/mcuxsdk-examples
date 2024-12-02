/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include <stdlib.h>

#include "fsl_debug_console.h"
#include "fsl_flash.h"
#include "fsl_common.h"

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
/*!
 * @brief Main function
 */
int main(void)
{
    flash_config_t config;
    bool pass       = true;
    status_t result = 0;
    uint32_t i      = 0;
    uint8_t data_buf[TEST_MEM_SIZE];

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();
    /* Configure Flash  */
    FLASH_GetDefaultConfig(&config);
    /*Flash module init*/
    FLASH_Init(&config);
    PRINTF("FLASH_Init\r\n");

    /* erase test page*/
    result = FLASH_Erase(&config, TEST_START_ADDR, FSL_FEATURE_FLASH_PAGE_SIZE_BYTES);
    if (result)
    {
        PRINTF("FLASH_PageErase error  result = %d \r\n", result);
        while (1)
        {
        }
    }
    /*set test data */
    for (i = 0; i < TEST_MEM_SIZE; i++)
    {
        data_buf[i] = (uint8_t)i;
    }
    result = FLASH_Program(&config, TEST_START_ADDR, (uint32_t *)&data_buf[0], TEST_MEM_SIZE - 1);
    if (result)
    {
        PRINTF("FLASH_Program error  result = %d \r\n", result);
        while (1)
        {
        }
    }
    if (memcmp((void *)TEST_START_ADDR, (uint8_t *)&data_buf[0], TEST_MEM_SIZE - 1))
    {
        pass = false;
    }
    /*show result*/
    if (pass)
    {
        PRINTF("Passed!\r\n");
    }
    else
    {
        PRINTF("Failed!\r\n");
    }

    while (1)
    {
    }
}
