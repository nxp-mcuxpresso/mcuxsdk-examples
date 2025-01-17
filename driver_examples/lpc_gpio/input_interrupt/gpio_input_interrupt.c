/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
static void delay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Whether the SW is turned on */
volatile bool g_InputSignal = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Interrupt service fuction of switch.
 */
void APP_GPIO_INTA_IRQHandler(void)
{
    /* clear the interrupt status */
    GPIO_PinClearInterruptFlag(GPIO, APP_SW_PORT, APP_SW_PIN, 0);
    /* Change state of switch. */
    g_InputSignal = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config    = {kGPIO_DigitalInput, 0};
    gpio_interrupt_config_t config = {kGPIO_PinIntEnableEdge, kGPIO_PinIntEnableLowOrFall};

    /* hardware initialiize, include IOMUX, Uart debug initialize */
    BOARD_InitHardware();
    PRINTF("GPIO Driver example.\r\n");

    /* Init input switch GPIO. */
    EnableIRQ(APP_SW_IRQ);
    GPIO_PortInit(GPIO, APP_SW_PORT);
    GPIO_PinInit(GPIO, APP_SW_PORT, APP_SW_PIN, &sw_config);

    /* Enable GPIO pin interrupt */
    GPIO_SetPinInterruptConfig(GPIO, APP_SW_PORT, APP_SW_PIN, &config);
    GPIO_PinEnableInterrupt(GPIO, APP_SW_PORT, APP_SW_PIN, 0);

    while (1)
    {
        if (g_InputSignal)
        {
            delay();
            if (APP_SW_CONNECTED_LEVEL == GPIO_PinRead(GPIO, APP_SW_PORT, APP_SW_PIN))
            {
                PRINTF("%s is turned on.\r\n", APP_SW_NAME);
            }
            /* Reset state of switch. */
            g_InputSignal = false;
        }
    }
}

static void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __NOP(); /* delay */
    }
}
