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
status_t APP_PCAL6524_Lock(bool lock)
{
    if (lock)
    {
        DisableIRQ(BOARD_PCAL6524_INT_IRQ);
    }
    else
    {
        EnableIRQ(BOARD_PCAL6524_INT_IRQ);
    }
    return kStatus_Success;
}

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_Init6524Pins();
    BOARD_InitLEDsPins();

    /* PCAL6524 INT is active-LOW open-drain → falling edge on the MCU side. */
    gpio_pin_config_t intPinConfig = {
        kGPIO_DigitalInput,
        0,
    };
    GPIO_PinInit(BOARD_PCAL6524_INT_GPIO, BOARD_PCAL6524_INT_PIN, &intPinConfig);
    GPIO_SetPinInterruptConfig(BOARD_PCAL6524_INT_GPIO, BOARD_PCAL6524_INT_PIN,
                               kGPIO_InterruptFallingEdge);
    GPIO_GpioClearInterruptFlags(BOARD_PCAL6524_INT_GPIO, 1U << BOARD_PCAL6524_INT_PIN);
    EnableIRQ(BOARD_PCAL6524_INT_IRQ);
}

/* If another component on this board uses a pin on the same GPIO controller,
 * its handler must be merged with this one. */
void BOARD_PCAL6524_INT_IRQ_HANDLER(void)
{
    uint32_t flags = GPIO_GpioGetInterruptFlags(BOARD_PCAL6524_INT_GPIO);

    if (0U != (flags & (1U << BOARD_PCAL6524_INT_PIN)))
    {
        GPIO_GpioClearInterruptFlags(BOARD_PCAL6524_INT_GPIO,
                                     1U << BOARD_PCAL6524_INT_PIN);
        g_pcal6524IntFlag = true;
    }
    SDK_ISR_EXIT_BARRIER;
}
/*${function:end}*/
