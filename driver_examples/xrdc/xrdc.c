/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "board.h"
#include "fsl_xrdc.h"
#include "app.h"
#include "fsl_debug_console.h"

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
 * @brief HardFault_Handler
 */
void HardFault_Handler(void)
{
    xrdc_error_t error;

    while (kStatus_Success == XRDC_GetAndClearFirstDomainError(XRDC, &error))
    {
        APP_CheckAndResolveMemoryAccessError(&error);
        APP_CheckAndResolvePeriphAccessError(&error);
        g_hardfaultFlag = true;
    }
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
    PRINTF("XRDC example start\r\n");

    /* Init the XRDC configuration, set the domain ID and access policy. */
    APP_SetXrdcConfig();

    /* Set XRDC global valid */
    XRDC_SetGlobalValid(XRDC, true);

    /* Set the unaccessible peripheral. */
    PRINTF("Set the peripheral not accessiable\r\n");
    APP_SetPeriphUnaccessible();

    /* Touch the peripheral, there will be hardfault. */
    g_hardfaultFlag = false;

    APP_TouchPeriph();

    /* Wait for the hardfault occurs. */
    while (!g_hardfaultFlag)
    {
    }
    PRINTF("The peripheral is accessiable now\r\n");

    /* Set the unaccessible memory region. */
    PRINTF("Set the memory not accessiable\r\n");
    APP_SetMemoryUnaccessible();

    /* Touch the memory, there will be hardfault. */
    g_hardfaultFlag = false;

    APP_TouchMemory();

    /* Wait for the hardfault occurs. */
    while (!g_hardfaultFlag)
    {
    }

    PRINTF("The memory is accessiable now\r\n");

    /* XRDC is global invalid */
    XRDC_SetGlobalValid(XRDC, false);

    PRINTF("XRDC example Success\r\n");

    while (1)
    {
    }
}
