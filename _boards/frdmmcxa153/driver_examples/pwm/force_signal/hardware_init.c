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
#include "fsl_gpio.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_GateGPIO3);
    RESET_ReleasePeripheralReset(kGPIO3_RST_SHIFT_RSTn);
    gpio_pin_config_t gpioConfig = {kGPIO_DigitalOutput, 1U};
    GPIO_PinInit(DEMO_PHASE_GPIO, DEMO_PHASE_GPIO_PIN, &gpioConfig);
}
/*${function:end}*/
