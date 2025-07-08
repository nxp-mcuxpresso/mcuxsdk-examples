/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file tickless_gpio_siul2_hal.c
 * @brief SIUL2 GPIO HAL implementation for FreeRTOS tickless mode
 *
 * This file implements the tickless_gpio_hal_t interface for the SIUL2 peripheral.
 *
 * @note This implementation requires the following board-specific macros to be defined
 * in the example's app.h file:
 *
 * - BOARD_SW_GPIO: The SIUL2 peripheral instance to use (e.g., SIUL2)
 * - BOARD_SW_GPIO_PIN: The pin number for the button (e.g., 9)
 * - BOARD_SW_IRQ: The IRQ number for the button interrupt (e.g., SIUL2_0_IRQn)
 *
 * Example definitions in app.h:
 * @code
 * #define BOARD_SW_GPIO SIUL2
 * #define BOARD_SW_GPIO_PIN 13U
 * #define BOARD_SW_IRQ SIUL2_1_IRQn
 * @endcode
 */

#include "tickless_hal.h"
#include "fsl_siul2.h"
#include "board.h"

#ifdef BOARD_SW_NAME

/* Initialize GPIO/button hardware */
static void siul2_init(void)
{
    /* Pin Initialization is done in hardware_init.c by BOARD_InitEXTIRQPins(); */

    /* Enable external interrupt */
    SIUL2_SetGlitchFilterPrescaler(BOARD_GPIO_BASE, 1U);
    SIUL2_EnableExtInterrupt(BOARD_GPIO_BASE, BOARD_SW_GPIO_PIN, kSIUL2_InterruptFallingEdge, 2U);
}

/* Get GPIO/button IRQ number */
static IRQn_Type siul2_get_irqn(void)
{
    return BOARD_SW_IRQ;
}

/* GPIO/button ISR handler */
static void siul2_isr_handler(void)
{
    /* Clear external interrupt flag */
    SIUL2_ClearExtDmaInterruptStatusFlags(BOARD_GPIO_BASE, (1U << BOARD_SW_GPIO_PIN));
}

/* Check if button is pressed */
static bool siul2_is_button_pressed(void)
{
    return (SIUL2_PinRead(BOARD_GPIO_BASE, BOARD_SW_GPIO_PIN) == 0);
}

/* SIUL2 HAL implementation */
static const tickless_gpio_hal_t siul2_hal = {.init              = siul2_init,
                                              .get_irqn          = siul2_get_irqn,
                                              .isr_handler       = siul2_isr_handler,
                                              .is_button_pressed = siul2_is_button_pressed};

#if defined(CONFIG_TICKLESS_GPIO_SIUL2)
const tickless_gpio_hal_t *tickless_get_gpio_hal(void)
{
    return &siul2_hal;
}
#endif
#endif
