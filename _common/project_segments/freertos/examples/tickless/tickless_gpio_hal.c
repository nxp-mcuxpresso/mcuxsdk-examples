/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tickless_hal.h"
#include "fsl_gpio.h"
#include "board.h"
#if defined(FSL_FEATURE_SOC_PORT_COUNT) && (FSL_FEATURE_SOC_PORT_COUNT > 0)
#include "fsl_port.h"
#endif

#if defined(FSL_FEATURE_SOC_PINT_COUNT) && (FSL_FEATURE_SOC_PINT_COUNT > 0)
#include "fsl_inputmux.h"
#include "fsl_pint.h"
#endif

/**
 * @file tickless_gpio_hal.c
 * @brief GPIO HAL implementation for FreeRTOS tickless mode
 *
 * This file implements the tickless_gpio_hal_t interface for the GPIO peripheral.
 *
 * @note This implementation requires the following board-specific macros to be defined
 * in the example's app.h file:
 *
 * - BOARD_SW_GPIO: The GPIO peripheral instance to use (e.g., GPIO1)
 * - BOARD_SW_GPIO_PIN: The pin number for the button (e.g., 9)
 * - BOARD_SW_IRQ: The IRQ number for the button interrupt (e.g., GPIO1_0_IRQn)
 * - BOARD_SW_INT_OUTPUT: The interrupt output to use (e.g., kGPIO_InterruptOutput0)
 *
 * Example definitions in app.h:
 * @code
 * #define BOARD_SW_GPIO GPIO1
 * #define BOARD_SW_GPIO_PIN 9
 * #define BOARD_SW_IRQ GPIO1_0_IRQn
 * #define BOARD_SW_INT_OUTPUT kGPIO_InterruptOutput0
 * @endcode
 *
 */

#ifdef BOARD_SW_NAME

/* Initialize GPIO/button hardware */
static void gpio_init(void)
{
/* Init input switch GPIO. */
#if defined(FSL_FEATURE_SOC_PINT_COUNT) && (FSL_FEATURE_SOC_PINT_COUNT > 0) && defined(BOARD_PINT_PIN_INT_SRC)
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, BOARD_PINT_PIN_INT_SRC);

    /* Initialize PINT */
    PINT_Init(PINT);

    /* Setup Pin Interrupt 0 for falling edge */
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge);
#else
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
#if defined(FSL_FEATURE_SOC_IGPIO_COUNT) && (FSL_FEATURE_SOC_IGPIO_COUNT > 0)
        kGPIO_IntRisingEdge,
#endif
    };
#if defined(FSL_FEATURE_SOC_PORT_COUNT) && (FSL_FEATURE_SOC_PORT_COUNT > 0)
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
    GPIO_SetPinInterruptConfig(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, kGPIO_InterruptFallingEdge);
#else
    PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
#endif
#endif

    GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
#if defined(FSL_FEATURE_SOC_IGPIO_COUNT) && (FSL_FEATURE_SOC_IGPIO_COUNT > 0)
    GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
    GPIO_PortEnableInterrupts(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
#endif
#endif
}

/* Get GPIO/button IRQ number */
static IRQn_Type gpio_get_irqn(void)
{
    return BOARD_SW_IRQ;
}

/* GPIO/button ISR handler */
static void gpio_isr_handler(void)
{
#if defined(FSL_FEATURE_SOC_PINT_COUNT) && (FSL_FEATURE_SOC_PINT_COUNT > 0) && defined(BOARD_PINT_PIN_INT_SRC)
    PINT_PinInterruptClrFallFlag(PINT, kPINT_PinInt0);
#else
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
    /* Clear external interrupt flag. */
    GPIO_GpioClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
#else
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
#endif
#endif
}

/* Check if button is pressed */
static bool gpio_is_button_pressed(void)
{
    return (1 == GPIO_PinRead(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN));
}

/* GPIO HAL implementation */
static const tickless_gpio_hal_t gpio_hal = {.init              = gpio_init,
                                             .get_irqn          = gpio_get_irqn,
                                             .isr_handler       = gpio_isr_handler,
                                             .is_button_pressed = gpio_is_button_pressed};

#if defined(CONFIG_TICKLESS_GPIO_STANDARD)
const tickless_gpio_hal_t *tickless_get_gpio_hal(void)
{
    return &gpio_hal;
}
#endif
#endif
