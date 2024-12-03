/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include <stdlib.h>

#include "fsl_debug_console.h"
#include "fsl_acmp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ACMP_USER_CHANNEL kACMP_Channel0
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t g_AcmpIntFlags = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/
void ACMP0_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(ACMP0_IRQn);
    g_AcmpIntFlags = 1;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    acmp_config_t config;
    BOARD_InitHardware();

    PRINTF("\r\nACMP example.\r\n");
    /*PA24--ACMP0N*/
    /*PA25--ACMP0P*/
    ACMP_GetDefaultConfig(&config);

    config.ch     = DEMO_ACMP_USER_CHANNEL;
    config.refDiv = kACMP_ReferenceVoltageDivider4; /*0.75V*/
    config.refSrc = kACMP_ReferenceSourceVcc;
    config.hystEn = kACMP_HysteresisEnable;

    ACMP_Init(DEMO_ACMP_BASE, &config);
    ACMP_EnableInterrupts(DEMO_ACMP_BASE, DEMO_ACMP_USER_CHANNEL);
    ACMP_Enable(DEMO_ACMP_BASE, DEMO_ACMP_USER_CHANNEL);
    NVIC_EnableIRQ(ACMP0_IRQn);

    while (1)
    {
        while (!g_AcmpIntFlags)
        {
        }
        PRINTF("\r\nACMP0 interrupt. Input voltage is higher than 0.75V\r\n");
        g_AcmpIntFlags = 0;
    }
}
