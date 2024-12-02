/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board_fusionf1.h"
#include "fsl_rgpio.h"
#include "fsl_clock.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
}
void LED_INIT()
{
    rgpio_pin_config_t led_config = {
        kRGPIO_DigitalOutput,
        0,
    };
    CLOCK_EnableClock(kCLOCK_RgpioA);

    RGPIO_PinInit(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN, &led_config);
}
void LED_TOGGLE()
{
    RGPIO_PortToggle(BOARD_LED_RGPIO, 1u << BOARD_LED_RGPIO_PIN);
}
/*${function:end}*/
