/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sdu_example.h"
#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"
#include "fsl_adapter_sdu.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t g_sdu_main_continue = 0;


/*******************************************************************************
 * Private Functions
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    status_t ret = 0;

    //while (!g_sdu_main_continue);

    BOARD_InitHardware();

    OSA_Init();

    ret = SDU_Init();
    if (ret != kStatus_Success)
    {
        PRINTF("Failed to initialize SDIO");
        return 0;
    }

    PRINTF("OSA_Start.\r\n");
    OSA_Start();

    return 0;
}
