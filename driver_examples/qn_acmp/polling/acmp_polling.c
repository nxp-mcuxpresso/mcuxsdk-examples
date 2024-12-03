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
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ACMP_USER_CHANNEL kACMP_Channel1
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
    acmp_config_t config;
    uint8_t status = 0;
    BOARD_InitHardware();

    /*LED init*/
    CLOCK_EnableClock(kCLOCK_Gpio);
    LED_RED_INIT(LOGIC_LED_OFF);
    LED_BLUE_INIT(LOGIC_LED_OFF);
    LED_GREEN_INIT(LOGIC_LED_OFF);

    PRINTF("\r\nACMP polling example.\r\n");
    /*PA29--ACMP1N*/
    /*PA30--ACMP1P*/
    ACMP_GetDefaultConfig(&config);

    config.ch     = DEMO_ACMP_USER_CHANNEL;
    config.refDiv = kACMP_ReferenceVoltageDivider4; /*0.75V*/
    config.refSrc = kACMP_ReferenceSourceVcc;
    config.hystEn = kACMP_HysteresisEnable;

    ACMP_Init(DEMO_ACMP_BASE, &config);
    ACMP_Enable(DEMO_ACMP_BASE, DEMO_ACMP_USER_CHANNEL);

    while (1)
    {
        status = ACMP_GetValue(DEMO_ACMP_BASE, DEMO_ACMP_USER_CHANNEL);
        if (status)
        {
            LED_RED_ON();
        }
        else
        {
            LED_RED_OFF();
        }
    }
}
