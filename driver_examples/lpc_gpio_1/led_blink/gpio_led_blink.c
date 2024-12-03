/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_gpio.h"
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

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Init LED device
 */
void LED_Init(void)
{
    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0};
    /* config led pin output mode */
    GPIO_PinInit(GPIO, EXAMPLE_LED_ROW_PIN, &led_config);
    GPIO_PinInit(GPIO, EXAMPLE_LED_COL_PIN, &led_config);
    GPIO_PinWrite(GPIO, EXAMPLE_LED_ROW_PIN, 0);
    GPIO_PinWrite(GPIO, EXAMPLE_LED_COL_PIN, 0);
}
/*!
 * @brief delay a while.
 */
void Delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 8000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* hardware initialiize, include IOMUX */
    BOARD_InitHardware();

    /* Init led device*/
    LED_Init();

    while (1)
    {
        GPIO_PortToggle(GPIO, 1U << EXAMPLE_LED_ROW_PIN);
        Delay();
    }
}
