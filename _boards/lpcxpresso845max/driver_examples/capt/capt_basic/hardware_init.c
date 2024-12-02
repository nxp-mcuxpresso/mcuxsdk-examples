/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_power.h"
#include "fsl_gpio.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Attach main clock to CAPT */
    CLOCK_Select(kCAPT_Clk_From_Fro);
    POWER_DisablePD(kPDRUNCFG_PD_ACMP);
    /* Select the main clock as source clock of USART0 (debug console) */
    CLOCK_Select(BOARD_DEBUG_USART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockFRO30M();
    BOARD_InitDebugConsole();
}

void BOARD_InitLED(void)
{
    GPIO_PortInit(GPIO, 0U);
    GPIO_PinInit(GPIO, 0U, 15U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0U});
    GPIO_PinInit(GPIO, 0U, 16U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0U});
    GPIO_PinInit(GPIO, 0U, 17U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0U});
    GPIO_PinInit(GPIO, 0U, 18U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0U});
    GPIO_PinInit(GPIO, 0U, 19U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0U});
    GPIO_PinInit(GPIO, 0U, 20U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0U});
    GPIO_PinInit(GPIO, 0U, 21U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0U});
    GPIO_PinInit(GPIO, 0U, 22U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0U});
    GPIO_PinInit(GPIO, 0U, 23U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0U});
}

void BOARD_LED_ON(uint32_t index)
{
    switch (index)
    {
        case 0U:
            GPIO_PortClear(GPIO, 0U, 1U << 15U);
            break;
        case 1U:
            GPIO_PortClear(GPIO, 0U, 1U << 16U);
            break;
        case 2U:
            GPIO_PortClear(GPIO, 0U, 1U << 17U);
            break;
        case 3U:
            GPIO_PortClear(GPIO, 0U, 1U << 18U);
            break;
        case 4U:
            GPIO_PortClear(GPIO, 0U, 1U << 19U);
            break;
        case 5U:
            GPIO_PortClear(GPIO, 0U, 1U << 20U);
            break;
        case 6U:
            GPIO_PortClear(GPIO, 0U, 1U << 21U);
            break;
        case 7U:
            GPIO_PortClear(GPIO, 0U, 1U << 22U);
            break;
        case 8U:
            GPIO_PortClear(GPIO, 0U, 1U << 23U);
            break;
    }
}

void BOARD_LED_OFF(void)
{
    GPIO_PortSet(GPIO, 0U, 1U << 15U);
    GPIO_PortSet(GPIO, 0U, 1U << 16U);
    GPIO_PortSet(GPIO, 0U, 1U << 17U);
    GPIO_PortSet(GPIO, 0U, 1U << 18U);
    GPIO_PortSet(GPIO, 0U, 1U << 19U);
    GPIO_PortSet(GPIO, 0U, 1U << 20U);
    GPIO_PortSet(GPIO, 0U, 1U << 21U);
    GPIO_PortSet(GPIO, 0U, 1U << 22U);
    GPIO_PortSet(GPIO, 0U, 1U << 23U);
}
/*${function:end}*/
