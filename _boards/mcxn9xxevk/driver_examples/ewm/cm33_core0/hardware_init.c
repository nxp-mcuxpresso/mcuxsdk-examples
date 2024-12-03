/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_gpio.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetupOsc32KClocking(kCLOCK_Osc32kToWake);
    CLOCK_AttachClk(kXTAL32K2_to_EWM0);
}

/*!
 * @brief Configure gpio as input for button
 *
 */
void gpio_configure(void)
{
    CLOCK_EnableClock(kCLOCK_Gpio0);

    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };
    GPIO_PinInit(SW_GPIO, SW_GPIO_PIN, &sw_config);
}

/*!
 * @brief Check if button is pressed.
 *
 * This function gets the state of button.
 *
 * @return 0 if button is not pressed.
 *         1 if button is pressed
 */
uint32_t is_key_pressed(void)
{
    return (GPIO_PinRead(SW_GPIO, SW_GPIO_PIN) == SW_GPIO_PRESSED_VALUE);
}

/*${function:end}*/
