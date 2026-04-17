/*
 * Copyright 2026 NXP
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
void DEMO_MBC_IRQHandler(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_hardfaultFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
#pragma interrupt alignsp saveall
/*!
 * @brief TRDC_IRQ Handler
 */
void DEMO_MBC_IRQHandler(void)
{
    g_hardfaultFlag = true;
    PRINTF("MBC memory access error\r\n");
    APP_ResolveMbcAccessError();
    SDK_ISR_EXIT_BARRIER;
}
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    /* Print the initial banner */
    PRINTF("MBC example start\r\n");

    APP_SetMbcGlobalConfig();

    /* Set the MBC inaccessible. */
    PRINTF("Set the MBC selected memory block not accessible\r\n");
    APP_SetMbcInaccessible();

    /* Touch the MBC, there will be a bus error interrupt. */
    g_hardfaultFlag = false;

    APP_TouchMbcMemory();

    /* Wait for the interrupt to occur. */
    while (!g_hardfaultFlag)
    {
    }

    PRINTF("The MBC selected block is accessible now\r\n");

    PRINTF("MBC example Success\r\n");

    while (1)
    {
    }
}
