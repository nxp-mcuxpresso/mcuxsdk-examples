/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_rgpio.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
void delay(void);

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
    /* Define the init structure for the output LED pin*/
    rgpio_pin_config_t led_config = {
        kRGPIO_DigitalOutput,
        0,
    };

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Print a note to terminal. */
    PRINTF("\r\n RGPIO Driver example\r\n");
    PRINTF("\r\n The LED is taking turns to shine.\r\n");

    /* Init output LED GPIO. */
    RGPIO_PinInit(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN, &led_config);

    while (1)
    {
        SDK_DelayAtLeastUs(1000000U, SystemCoreClock);
        RGPIO_PortToggle(BOARD_LED_RGPIO, 1u << BOARD_LED_RGPIO_PIN);
    }
}
