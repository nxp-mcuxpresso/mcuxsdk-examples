/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
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
volatile bool g_BodIntFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void APP_BOD_IRQHander(void)
{
    APP_DeinitBod();
    g_BodIntFlag = true;
}

int main(void)
{
    BOARD_InitHardware();

    PRINTF("\r\nBOD INTERRUPT EXAMPLE.\r\n");

    APP_InitBod();
    NVIC_EnableIRQ(APP_BOD_IRQn);

    PRINTF("Please adjust input voltage low than %s to trigger BOD interrupt.\r\n", APP_BOD_THRESHOLD_VOLTAGE);
    while (!g_BodIntFlag)
    {
    }
    g_BodIntFlag = false;
    PRINTF("\r\nBOD interrupt occurred, input voltage is low than %s.\r\n", APP_BOD_THRESHOLD_VOLTAGE);
    while (1)
    {
    }
}
