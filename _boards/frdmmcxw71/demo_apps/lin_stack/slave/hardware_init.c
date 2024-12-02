/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };

    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

    BOARD_InitPins();
    BOARD_DebugConsolePins();
    BOARD_InitLeds();
    BOARD_InitButtons();
    BOARD_InitLinLpuart();
    BOARD_BootClockRUN();
    /* Set LIN LPUART clock */
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcFro6M);
#if defined(DEMO_TIMER_TPM_ENABLE) && DEMO_TIMER_TPM_ENABLE
    /* Set Timer LPUART clock */
    CLOCK_SetIpSrc(DEMO_TPM_CLOCK, kCLOCK_IpSrcFro6M);
#endif
    BOARD_InitDebugConsole();

    /* Initialize buttons */
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
    GPIO_SetPinInterruptConfig(DEMO_BUTTON1_GPIO, DEMO_BUTTON1_PIN, kGPIO_InterruptFallingEdge);
    GPIO_SetPinInterruptConfig(DEMO_BUTTON2_GPIO, DEMO_BUTTON2_PIN, kGPIO_InterruptFallingEdge);
#else
    PORT_SetPinInterruptConfig(DEMO_BUTTON1_PORT, DEMO_BUTTON1_PIN, kPORT_InterruptFallingEdge);
    PORT_SetPinInterruptConfig(DEMO_BUTTON2_PORT, DEMO_BUTTON2_PIN, kPORT_InterruptFallingEdge);
#endif
    GPIO_PinInit(DEMO_BUTTON1_GPIO, DEMO_BUTTON1_PIN, &sw_config);
    GPIO_PinInit(DEMO_BUTTON2_GPIO, DEMO_BUTTON2_PIN, &sw_config);
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
    GPIO_GpioClearInterruptFlags(DEMO_BUTTON1_GPIO, 1U << DEMO_BUTTON1_PIN);
    GPIO_GpioClearInterruptFlags(DEMO_BUTTON2_GPIO, 1U << DEMO_BUTTON2_PIN);
#else
    GPIO_PortClearInterruptFlags(DEMO_BUTTON1_GPIO, 1U << DEMO_BUTTON1_PIN);
    GPIO_PortClearInterruptFlags(DEMO_BUTTON2_GPIO, 1U << DEMO_BUTTON2_PIN);
#endif
    /* Enable the NVIC of button. */
    EnableIRQ(DEMO_SW2_IRQn);
    EnableIRQ(DEMO_SW3_IRQn);

    /* Initizlize LEDs. */
    GPIO_PinInit(DEMO_LED1_GPIO, DEMO_LED1_PIN, &led_config);
    GPIO_PinInit(DEMO_LED2_GPIO, DEMO_LED2_PIN, &led_config);
    GPIO_PinInit(DEMO_LED3_GPIO, DEMO_LED3_PIN, &led_config);
}
/*${function:end}*/
