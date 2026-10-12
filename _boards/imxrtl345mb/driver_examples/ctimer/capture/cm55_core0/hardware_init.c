/*
 * Copyright 2025-2026 NXP
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
#include <stdbool.h>
#include "fsl_reset.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    BOARD_InitCtimerCapPins();
}

/* Route the GPIO to ctimer capture. */
void DEMO_InitCtimerInput(void)
{
    /* Connect pin set as TRIG_IN3 to CTIMER2 capture */
    INPUTMUX_Init(CTRLSS__INPUTMUX_0);
    INPUTMUX_AttachSignal(CTRLSS__INPUTMUX_0, kINPUTMUX_INDEX_TIMER2CAPTSEL0, kINPUTMUX_IoPCtCapt2_3ToTIMER2CAPTSELn);
}

void DEMO_InitGpioPin(void)
{
    const gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 0,
    };

    /* Init to low level. */
    GPIO_PinInit(CTRLSS__GPIO, DEMO_GPIO, DEMO_GPIO_PIN, &pinConfig);
}

void DEMO_PullGpioPin(uint8_t level)
{
    GPIO_PinWrite(CTRLSS__GPIO, DEMO_GPIO, DEMO_GPIO_PIN, level);
}
/*${function:end}*/