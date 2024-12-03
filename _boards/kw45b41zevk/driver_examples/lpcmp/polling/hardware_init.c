/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_vref.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    vref_config_t vrefConfig;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    gpio_pin_config_t LED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 0U,
    };

    /* Initialize GPIO functionality on pin PTB1 */
    GPIO_PinInit(BOARD_LED1_GPIO, BOARD_LED1_GPIO_PIN, &LED_config);

    VREF_GetDefaultConfig(&vrefConfig);
    /* Initialize the VREF mode. */
    VREF_Init(DEMO_VREF_BASE, &vrefConfig);
    /* Get a 1.8V reference voltage. */
    VREF_SetTrim21Val(DEMO_VREF_BASE, 8U);
}
/*${function:end}*/
