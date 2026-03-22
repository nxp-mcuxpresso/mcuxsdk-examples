/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitButtonsPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Intm);
}

void BOARD_InitSWConfig(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };

    /* Init input switch GPIO */
    GPIO_SetPinInterruptConfig(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, kGPIO_InterruptFallingEdge);

    /* Enable interrupt*/
    EnableIRQ(BOARD_SW_IRQ);

    /* GPIO pin init */
    GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
}

void BOARD_ClearSWIntFlag(void)
{
    GPIO_GpioClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
}
/*${function:end}*/
