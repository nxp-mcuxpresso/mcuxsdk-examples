/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tickless_hal.h"
#include "fsl_gpio.h"
#include "fsl_inputmux.h"
#include "fsl_pint.h"
#include "board.h"

/**
 * @file tickless_gpio_lpc_hal.c
 * @brief LPC GPIO HAL implementation for FreeRTOS tickless mode
 *
 * This file implements the tickless_gpio_hal_t interface for the LPC GPIO peripheral
 * using PINT (Pin Interrupt) functionality.
 *
 * @note This implementation requires the following board-specific macros to be defined
 * in the example's app.h file:
 *
 * - BOARD_SW_GPIO: The GPIO peripheral instance to use (e.g., GPIO)
 * - BOARD_SW_GPIO_PORT: The GPIO port number (e.g., 0)
 * - BOARD_SW_GPIO_PIN: The pin number for the button (e.g., 4)
 * - BOARD_SW_IRQ: The IRQ number for the button interrupt (e.g., PIN_INT0_IRQn)
 * - BOARD_PINT_PIN_INT_SRC: The PINT pin interrupt source (e.g., kINPUTMUX_GpioPort0Pin4ToPintsel)
 *
 * Example definitions in app.h:
 * @code
 * #define BOARD_SW_GPIO GPIO
 * #define BOARD_SW_GPIO_PORT 0
 * #define BOARD_SW_GPIO_PIN 4
 * #define BOARD_SW_IRQ PIN_INT0_IRQn
 * #define BOARD_PINT_PIN_INT_SRC kINPUTMUX_GpioPort0Pin4ToPintsel
 * @endcode
 */

#ifdef BOARD_SW_NAME

/* Initialize LPC GPIO/button hardware */
static void lpc_gpio_init(void)
{
    /* Connect trigger sources to PINT */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, BOARD_PINT_PIN_INT_SRC);

    /* Initialize PINT */
    PINT_Init(PINT);

    /* Setup Pin Interrupt 0 for falling edge */
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge);

    /* Enable callbacks for PINT */
    PINT_EnableCallback(PINT);
}

/* Get LPC GPIO/button IRQ number */
static IRQn_Type lpc_gpio_get_irqn(void)
{
    return BOARD_SW_IRQ;
}

/* LPC GPIO/button ISR handler */
static void lpc_gpio_isr_handler(void)
{
    /* Clear external interrupt flag */
    PINT_PinInterruptClrFallFlag(PINT, kPINT_PinInt0);
}

/* Check if button is pressed */
static bool lpc_gpio_is_button_pressed(void)
{
    return (0 == GPIO_PinRead(BOARD_SW_GPIO, BOARD_SW_GPIO_PORT, BOARD_SW_GPIO_PIN));
}

/* LPC GPIO HAL implementation */
static const tickless_gpio_hal_t lpc_gpio_hal = {.init              = lpc_gpio_init,
                                                 .get_irqn          = lpc_gpio_get_irqn,
                                                 .isr_handler       = lpc_gpio_isr_handler,
                                                 .is_button_pressed = lpc_gpio_is_button_pressed};

#if defined(CONFIG_TICKLESS_GPIO_LPC)
const tickless_gpio_hal_t *tickless_get_gpio_hal(void)
{
    return &lpc_gpio_hal;
}
#endif

#endif
