/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_BootClockRUN();
    BOARD_InitDebugConsolePins();
    BOARD_InitDebugConsole();

    if (BOARD_IsLowPowerBootType() != true) /* not low power boot type */
    {
        BOARD_HandshakeWithUboot();
    }
    else /* low power boot type */
    {
        PRINTF("Pls run the demo with A Core(the demo depend on resource of A Core)\r\n");
        assert(false);
    }

    SDK_DelayAtLeastUs(4000000U, SystemCoreClock); /* wait 4 seconds to make sure setup gpio by mcore */

    BOARD_InitBootPins();
    CLOCK_EnableClock(kCLOCK_RgpioF);

    /* Make m33(in secure world, private mode) can write RGPIOE/F(TF-A running on A Core changed gpio's PCNS, ICNS,
     * PCNP, ICNP, this make master(m33) in nonsecure world, user mode can write gpio) */
    if (BOARD_LED_RGPIO->PCNS & (1 << BOARD_LED_RGPIO_PIN))
    {
        BOARD_LED_RGPIO->PCNS &=
            ~(1 << BOARD_LED_RGPIO_PIN); /* make master in secure world, private mode can access the pin */
    }
    if (BOARD_LED_RGPIO->ICNS & (1 << BOARD_LED_RGPIO_PIN))
    {
        BOARD_LED_RGPIO->ICNS &=
            ~(1 << BOARD_LED_RGPIO_PIN); /* make master in secure world, private mode can access the pin */
    }
    if (BOARD_LED_RGPIO->PCNP & (1 << BOARD_LED_RGPIO_PIN))
    {
        BOARD_LED_RGPIO->PCNP &=
            ~(1 << BOARD_LED_RGPIO_PIN); /* make master in secure world, private mode can access the pin */
    }
    if (BOARD_LED_RGPIO->ICNP & (1 << BOARD_LED_RGPIO_PIN))
    {
        BOARD_LED_RGPIO->ICNP &=
            ~(1 << BOARD_LED_RGPIO_PIN); /* make master in secure world, private mode can access the pin */
    }
}
/*${function:end}*/
