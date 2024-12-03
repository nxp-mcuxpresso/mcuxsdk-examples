/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
volatile bool g_bButtonPressed = false;

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
void ButtonIrqHandler();
void ButtonIrqHandler()
{
    if (GPIO_PortGetEdgeDetectedStatusFlags(DEMO_BUTTON_GPIO) & DEMO_BUTTON_GPIO_PIN)
    {
        /* Clear Status Flags */
        GPIO_PortClearEdgeDetectedStatusFlags(DEMO_BUTTON_GPIO, DEMO_BUTTON_GPIO_PIN);
        g_bButtonPressed = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Define the init structure for the output LED pin*/
    gpio_config_t sLedConfig = {
        .eDirection     = kGPIO_DigitalOutput,
        .eMode          = kGPIO_ModeGpio,
        .eOutMode       = kGPIO_OutputPushPull,
        .eSlewRate      = kGPIO_SlewRateFast,
        .eOutLevel      = kGPIO_OutputHigh,
        .eDriveStrength = kGPIO_DriveStrengthHigh,
        .ePull          = kGPIO_PullDisable,
        .eInterruptMode = kGPIO_InterruptDisable,
    };

    gpio_config_t sButtonConfig = {
        .eDirection     = kGPIO_DigitalInput,
        .eMode          = kGPIO_ModeGpio,
        .eOutMode       = kGPIO_OutputPushPull,
        .eSlewRate      = kGPIO_SlewRateFast,
        .eOutLevel      = kGPIO_OutputHigh,
        .eDriveStrength = kGPIO_DriveStrengthHigh,
        .ePull          = kGPIO_PullDisable,
        .eInterruptMode = kGPIO_InterruptFallingEdge,
    };
    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Print a note to terminal. */
    PRINTF("\r\n GPIO Driver example\r\n");
    PRINTF("\r\n Press the button SW2 to toggle the LED.\r\n");

    /* Init output LED GPIO. */
    GPIO_PinInit(DEMO_LED_GPIO, DEMO_LED_GPIO_PIN, &sLedConfig);
    GPIO_PinInit(DEMO_BUTTON_GPIO, DEMO_BUTTON_GPIO_PIN, &sButtonConfig);

    /* Enable Interrupt. */
    EnableIRQWithPriority(DEMO_BUTTON_IRQn, DEMO_BUTTON_PRIO);

    while (1)
    {
        if (g_bButtonPressed == true)
        {
            GPIO_PinToggle(DEMO_LED_GPIO, DEMO_LED_GPIO_PIN);
            g_bButtonPressed = false;
            PRINTF("\r\n Button press event detected....\r\n");
        }
    }
}
