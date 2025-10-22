/*
 * Copyright 2025 NXP
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
    BOARD_InitBootPins();
    BOARD_InitCtimerCapPins();
    BOARD_InitDebugConsole();

    /* CTIMER2 used by this example won't work without CTIMER1 released from reset and clocked */

    /* Enable clock */
    CLOCK_EnableClock(kCLOCK_GateCTIMER1);

    /* Release peripheral reset */
    RESET_ClearPeripheralReset(kCTIMER1_RST_SHIFT_RSTn);
}

/* Route the GPIO to ctimer capture. */
void DEMO_InitCtimerInput(void)
{
    /* Connect pin set as TRIG_IN3 to CTIMER2 capture */
    INPUTMUX_Init(INPUTMUX0);
    INPUTMUX_AttachSignal(INPUTMUX0, kINPUTMUX_INDEX_CTIMER2CAP0, kINPUTMUX_TrigInp3ToCTIMER2CAPn);
}

void DEMO_InitGpioPin(void)
{
    const gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 0,
    };

    /* Init to low level. */
    GPIO_PinInit(DEMO_GPIO, (uint32_t)DEMO_GPIO_PIN, &pinConfig);
}

void DEMO_PullGpioPin(uint8_t level)
{
    GPIO_PinWrite(DEMO_GPIO, (uint32_t)DEMO_GPIO_PIN, level);
}
/*${function:end}*/