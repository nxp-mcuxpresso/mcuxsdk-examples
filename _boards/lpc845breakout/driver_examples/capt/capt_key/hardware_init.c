/*
 * Copyright  2020 NXP
 * All rights reserved.
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

/*${variable:start}*/
static const uint8_t ledPinIndex[] = {0};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Attach clock to CAPT */
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
    const gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1U,
    };

    GPIO_PortInit(GPIO, 1U);

    for (uint8_t i = 0; i < ARRAY_SIZE(ledPinIndex); ++i)
    {
        GPIO_PinInit(GPIO, 1U, ledPinIndex[i], &pinConfig);
    }
}

void BOARD_TurnOnLed(uint32_t index)
{
    if (index < ARRAY_SIZE(ledPinIndex))
    {
        GPIO_PortClear(GPIO, 1U, 1U << ledPinIndex[index]);
    }
}

void BOARD_TurnOffLed(uint32_t index)
{
    if (index < ARRAY_SIZE(ledPinIndex))
    {
        GPIO_PortSet(GPIO, 1U, 1U << ledPinIndex[index]);
    }
}

void BOARD_TurnOnAllLed(void)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(ledPinIndex); ++i)
    {
        GPIO_PortClear(GPIO, 1U, 1U << ledPinIndex[i]);
    }
}

void BOARD_TurnOffAllLed(void)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(ledPinIndex); ++i)
    {
        GPIO_PortSet(GPIO, 1U, 1U << ledPinIndex[i]);
    }
}

/*${function:end}*/
