/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "fsl_gpio.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    gpio_pin_config_t config = {
        kGPIO_DigitalOutput,
        0,
    };

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Initialize for the output gpio signal. */
    GPIO_PinInit(EXAMPLE_KBI_SIGNAL_INPUT_REF_GPIO, EXAMPLE_KBI_SIGNAL_INPUT_REF_GPIO_INDEX, &config);
}
/*${function:end}*/
