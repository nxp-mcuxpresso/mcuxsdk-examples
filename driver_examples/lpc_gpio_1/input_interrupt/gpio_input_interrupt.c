/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_gpio.h"
#include "fsl_common.h"
#include "app.h"
#include "pin_mux.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool g_InputSignal = false; /* Whether the SW is turned on */

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
static void Delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000; ++i)
    {
        __NOP(); /* delay */
    }
}

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
 * @brief Interrupt service fuction of switch.
 */
void EXAMPLE_GPIO_IRQHandler(void)
{
    /* Change state of switch. */
    if ((GPIO_PortGetInterruptsFlags(GPIO) & ((uint32_t)(1 << EXAMPLE_SW_GPIO_PIN))) &&
        (GPIO_PortReadMaskedInterrupts(GPIO) & ((uint32_t)(1 << EXAMPLE_SW_GPIO_PIN))))
    {
        g_InputSignal = true;
    }

    /* clear the interrupt status */
    GPIO_PortClearInterruptFlags(GPIO, 1U << EXAMPLE_SW_GPIO_PIN);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };

    /* hardware initialiize, include IOMUX */
    BOARD_InitHardware();

    /* Init led device */
    LED_Init();

    /* Init input switch GPIO. */
    EnableIRQ(EXAMPLE_SW_IRQ);
    GPIO_PinInit(GPIO, EXAMPLE_SW_GPIO_PIN, &sw_config);

    /* config sw pin interrupt mode*/
    GPIO_PinSetInterruptConfig(GPIO, EXAMPLE_SW_GPIO_PIN, kGPIO_IntRisingEdge);

    /* Enable GPIO pin interrupt */
    GPIO_PortEnableInterrupts(GPIO, 1U << EXAMPLE_SW_GPIO_PIN);

    while (1)
    {
        if (g_InputSignal)
        {
            GPIO_PortToggle(GPIO, 1 << EXAMPLE_LED_ROW_PIN);
            Delay();
        }
    }
}
