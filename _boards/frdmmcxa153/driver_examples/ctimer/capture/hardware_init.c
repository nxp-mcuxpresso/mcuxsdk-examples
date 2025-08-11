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
    /* CTimer functional clock needs to be greater than or equal to SYSTEM_CLK */
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER0, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER0);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

/* Route the GPIO to ctimer capture. */
void DEMO_InitCtimerInput(void)
{
    /* INPUTMUX0: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateINPUTMUX0);
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);

    /*  CtimerInp8 connect to Timer0Captsel 0 */
    INPUTMUX_Init(INPUTMUX0);
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_CtimerInp8ToTimer0Captsel);
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
