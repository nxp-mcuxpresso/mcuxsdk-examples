/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
/*${header:end}*/

/*${function:start}*/

extern pcal6524_handle_t s_ioExpHandle;

static void APP_IOEXP_GpioCallback(GPIO_Type *base, gpio_handle_t *handle, uint32_t pinMask, void *userData)
{
    PCAL6524_InterruptHandler(&s_ioExpHandle);
}

void BOARD_InitHardware(void)
{
    BOARD_CommonSetting();
    BOARD_Init6524Pins();
    BOARD_InitLEDsPins();

    const gpio_pin_config_t intPinConfig = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0U,
    };
    GPIO_PinInit(BOARD_PCAL6524_INT_GPIO, BOARD_PCAL6524_INT_PIN, &intPinConfig);
    GPIO_SetPinInterruptConfig(BOARD_PCAL6524_INT_GPIO, BOARD_PCAL6524_INT_PIN,
                               kGPIO_InterruptFallingEdge);
    GPIO_GpioClearInterruptFlags(BOARD_PCAL6524_INT_GPIO, 1U << BOARD_PCAL6524_INT_PIN);
    /*
     * Register IOEXP Interrupt handler to GPIO interrupt dispatcher, so that when
     * GPIO pin interrupt happens, IOEXP interrupt handler can be called, then
     * dispatch interrupt to the modules connected to IOEXP.
     */
    GPIO_SetPinCallback(&g_gpio0Handle, 1U << BOARD_PCAL6524_INT_PIN, APP_IOEXP_GpioCallback, NULL);
}
/*${function:end}*/
