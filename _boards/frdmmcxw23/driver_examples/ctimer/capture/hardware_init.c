/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_inputmux.h"
#include "app.h"

/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    POWER_Init();

    CLOCK_EnableClock(kCLOCK_Iocon);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    GPIO_PortInit(GPIO, 0);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
    CLOCK_EnableClock(kCLOCK_FlexComm0);
    CLOCK_AttachClk(kMAIN_CLK_to_CTIMER0);

    BOARD_InitPins();
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
    INPUTMUX_AttachSignal(INPUTMUX, 0U, kINPUTMUX_CtimerInp15ToTimer0Captsel);
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
