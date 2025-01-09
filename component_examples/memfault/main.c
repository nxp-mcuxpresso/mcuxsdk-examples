/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "memfault/components.h"

extern void genhal_init();

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
    /* Init board hardware. */
    BOARD_InitHardware();
    genhal_init();
    memfault_platform_boot();

    int result = memfault_self_test_run(
                           kMemfaultSelfTestFlag_CoredumpRegions |
                           kMemfaultSelfTestFlag_CoredumpStorage |
                           kMemfaultSelfTestFlag_CoredumpStorageCapacity |
                           kMemfaultSelfTestFlag_DataExport);

    if (result == 0)
    {
        PRINTF("Test successed\r\n");
    }
    else
    {
        PRINTF("Test failed\r\n");
    }

    while(1)
    {
    }
}

