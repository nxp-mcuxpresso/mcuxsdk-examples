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
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_Init6524Pins();
    BOARD_InitLEDsPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* PCAL6524 INT is active-LOW open-drain → falling edge on the MCU side. */
    gpio_pin_config_t intPinConfig = {
        .direction     = kGPIO_DigitalInput,
        .outputLogic   = 0U,
        .interruptMode = kGPIO_IntFallingEdge,
    };
    GPIO_PinInit(BOARD_PCAL6524_INT_GPIO, BOARD_PCAL6524_INT_PIN, &intPinConfig);
    GPIO_ClearPinsInterruptFlags(BOARD_PCAL6524_INT_GPIO, 1U << BOARD_PCAL6524_INT_PIN);
    GPIO_EnableInterrupts(BOARD_PCAL6524_INT_GPIO, 1U << BOARD_PCAL6524_INT_PIN);
    EnableIRQ(BOARD_PCAL6524_INT_IRQ);
}

/* GPIO3_Combined_16_31_IRQn covers all GPIO3 pins 16..31; if another
 * component on this board uses one, merge its handler with this. */
void BOARD_PCAL6524_INT_IRQ_HANDLER(void)
{
    uint32_t flags = GPIO_GetPinsInterruptFlags(BOARD_PCAL6524_INT_GPIO);

    if (0U != (flags & (1U << BOARD_PCAL6524_INT_PIN)))
    {
        GPIO_ClearPinsInterruptFlags(BOARD_PCAL6524_INT_GPIO,
                                     1U << BOARD_PCAL6524_INT_PIN);
        g_pcal6524IntFlag = true;
    }
    SDK_ISR_EXIT_BARRIER;
}
/*${function:end}*/
