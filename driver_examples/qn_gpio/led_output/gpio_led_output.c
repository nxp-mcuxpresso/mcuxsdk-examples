/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
static void delay(uint32_t dly)
{
    volatile uint32_t i = 0;
    for (i = 0; i < dly; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

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
    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Print a note to terminal. */
    PRINTF("\r\n GPIO Driver example\r\n");
    PRINTF("\r\n The LED is taking turns to shine.\r\n");

    /* Enable GPIO clock */
    CLOCK_EnableClock(kCLOCK_Gpio);

/* Init output LED GPIO. */
#if ((defined(CFG_BOARD_TYPE)) && (CFG_BOARD_TYPE == BOARD_QN908X_DK))
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_RED_GPIO_PIN, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0U});
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GREEN_GPIO_PIN, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0U});
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_BLUE_GPIO_PIN, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0U});
#elif ((defined(CFG_BOARD_TYPE)) && (CFG_BOARD_TYPE == BOARD_QN908X_EVB))
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED1_GPIO_PIN, &(gpio_pin_config_t){kGPIO_DigitalOutput, 1U});
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED2_GPIO_PIN, &(gpio_pin_config_t){kGPIO_DigitalOutput, 1U});
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED3_GPIO_PIN, &(gpio_pin_config_t){kGPIO_DigitalOutput, 1U});
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED4_GPIO_PIN, &(gpio_pin_config_t){kGPIO_DigitalOutput, 1U});
#endif

    while (1)
    {
#if ((defined(CFG_BOARD_TYPE)) && (CFG_BOARD_TYPE == BOARD_QN908X_DK))
        delay(500000);
        GPIO_PortToggle(BOARD_LED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN);
#elif ((defined(CFG_BOARD_TYPE)) && (CFG_BOARD_TYPE == BOARD_QN908X_EVB))
        GPIO_PortToggle(BOARD_LED_GPIO, 1U << BOARD_LED1_GPIO_PIN);
        delay(100000);
        GPIO_PortToggle(BOARD_LED_GPIO, 1U << BOARD_LED2_GPIO_PIN);
        delay(100000);
        GPIO_PortToggle(BOARD_LED_GPIO, 1U << BOARD_LED3_GPIO_PIN);
        delay(100000);
        GPIO_PortToggle(BOARD_LED_GPIO, 1U << BOARD_LED4_GPIO_PIN);
        delay(100000);
#endif
    }
}
