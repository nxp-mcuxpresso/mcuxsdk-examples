/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    const gpio_pin_config_t resetPinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1,
    };
    
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1u);
    CLOCK_AttachClk(kPLL0_to_FLEXIO);

    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio4);

    /* Set SSD1963 CS, RS, and reset pin to output. */
    GPIO_PinInit(DEMO_SSD1963_RST_GPIO, DEMO_SSD1963_RST_PIN, &resetPinConfig);
    GPIO_PinInit(DEMO_SSD1963_CS_GPIO, DEMO_SSD1963_CS_PIN, &resetPinConfig);
    GPIO_PinInit(DEMO_SSD1963_RS_GPIO, DEMO_SSD1963_RS_PIN, &resetPinConfig);
}

void BOARD_SetCSPin(bool set)
{
    GPIO_PinWrite(DEMO_SSD1963_CS_GPIO, DEMO_SSD1963_CS_PIN, (uint8_t)set);
}

void BOARD_SetRSPin(bool set)
{
    GPIO_PinWrite(DEMO_SSD1963_RS_GPIO, DEMO_SSD1963_RS_PIN, (uint8_t)set);
}

void BOARD_SetResetPin(bool set)
{
    GPIO_PinWrite(DEMO_SSD1963_RST_GPIO, DEMO_SSD1963_RST_PIN, (uint8_t)set);
}
/*${function:end}*/
