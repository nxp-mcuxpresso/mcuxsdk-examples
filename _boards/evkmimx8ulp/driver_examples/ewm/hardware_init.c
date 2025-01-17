/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_rgpio.h"
#include "app.h"
/*${header:end}*/

/********************** End of forword declaration. ***************/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    CLOCK_EnableClock(kCLOCK_RgpioB);

    if (BOARD_IsLowPowerBootType() != true) /* not low power boot type */
    {
        BOARD_HandshakeWithUboot(); /* Must handshake with uboot, unless will get issues(such as: SoC reset all the
                                       time) */
    }
    else                            /* low power boot type */
    {
        BOARD_SetTrdcGlobalConfig();
    }
}
/*!
 * @brief Configure gpio as input for button
 *
 */
void gpio_configure(void)
{
    rgpio_pin_config_t sw_config = {
        kRGPIO_DigitalInput,
        0,
    };
    RGPIO_PinInit(SW_GPIO, SW_GPIO_PIN, &sw_config);
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
    return (RGPIO_PinRead(SW_GPIO, SW_GPIO_PIN) == SW_GPIO_PRESSED_VALUE);
}
/*${function:end}*/
