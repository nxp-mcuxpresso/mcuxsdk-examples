/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_inputmux.h"
#include "fsl_gpio.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Use 16 MHz clock for the Ctimer0 */
    CLOCK_AttachClk(kSFRO_to_CTIMER0);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

/* Route the GPIO to ctimer capture. */
void DEMO_InitCtimerInput(void)
{
    INPUTMUX_Init(INPUTMUX);

    /*
     * Connect INP11 to Channel 0. INP11 is GPIO51 is configured in pin_mux.c.
     */
    INPUTMUX_AttachSignal(INPUTMUX, 0U, kINPUTMUX_Gpio51Inp11ToTimer0CaptureChannels);
}

void DEMO_InitGpioPin(void)
{
    const gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 0,
    };

    /* Init to low level. */
    GPIO_PinInit(GPIO, DEMO_GPIO_PORT, DEMO_GPIO_PIN, &pinConfig);
}

void DEMO_PullGpioPin(int level)
{
    GPIO_PinWrite(GPIO, DEMO_GPIO_PORT, DEMO_GPIO_PIN, (uint8_t)level);
}
/*${function:end}*/
