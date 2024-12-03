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
#include "fsl_bod.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t g_BodIntFlags = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/
void BOD_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(BOD_IRQn);
    g_BodIntFlags = 1;
    NVIC_DisableIRQ(BOD_IRQn);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    bod_config_t config;
    BOARD_InitHardware();

    PRINTF("\r\nBOD example.\r\n");

    /* Check Reset source(Brown-Down reset source will be set when voltage is less than the user-defined) */
    if (kRESET_SrcBrownDown == RESET_GetResetSource())
    {
        PRINTF("Brown-Down reset occurred\r\n");
    }
    /* Clear reset source */
    RESET_ClearResetSource();

    BOD_GetDefaultConfig(&config);

    config.int_thr   = kBOD_InterruptThreshold2; /*2.72V*/
    config.reset_thr = kBOD_ResetThreshold2;     /*2.0V*/

    /*Init BOD module*/
    BOD_Init(DEMO_BOD_BASE, &config);
    BOD_Enable(DEMO_BOD_BASE, kBOD_InterruptEnable | kBOD_ResetEnable);
    NVIC_EnableIRQ(BOD_IRQn);

    while (!g_BodIntFlags)
    {
    }
    PRINTF("\r\nBOD interrupt. Input voltage is lower than 2.72V\r\n");

    PRINTF("CPU will be reset when input voltage is lower than 2.0V\r\n");

    while (1)
    {
    }
}
