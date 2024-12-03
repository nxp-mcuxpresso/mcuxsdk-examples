/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
static void BOARD_InitLedPin(void)
{
    const gpio_pin_config_t config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1,
    };

    GPIO_PinInit(BOARD_LED1_GPIO, BOARD_LED1_GPIO_PIN, &config);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitLedPin();
    NVIC_EnableIRQ(MUA_IRQn);
}
/*${function:end}*/
