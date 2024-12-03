/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
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
volatile bool g_hardfaultFlag = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief BusFault_Handler
 */
void BusFault_Handler(void)
{
    /* Enlarge the logical window */
    TRDC_GetSetFlashLogicalWindowSize(TRDC, EXAMPLE_TRDC_FLW_BLK_COUNT + 1U);
    PRINTF("Violent access at address: 0x%8X\r\n", SCB->BFAR);
    g_hardfaultFlag = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware.*/
    BOARD_InitHardware();

    /* Print the initial banner */
    PRINTF("TRDC flash logical window example start\r\n");

    /* Set the flash logical window. */
    PRINTF("Set the flash logical window\r\n");
    APP_SetTrdcFlwConfig();

    /* Touch memory beyond the window */
    APP_TouchMemory();

    /* Wait for the hardfault occurs. */
    while (!g_hardfaultFlag)
    {
    }

    PRINTF("The flash memory is accessiable now\r\n");

    if (APP_CheckMemoryMap())
    {
        PRINTF("The data between physical address window and programmable flash address are identical.\r\n");
    }
    else
    {
        PRINTF("The data between physical address window and programmable flash address are not identical!\r\n");
        return -1;
    }

    PRINTF("TRDC flash logical window example success\r\n");

    while (1)
    {
    }
}
