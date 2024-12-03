/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/* ${header:start} */
#include "fsl_common.h"
#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
/* ${header:end} */

/* ${function:start} */
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_ACCEL_Reset();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();

    /* Select the clock source for the FTM counter as fast internal RC oscillator */
    CLOCK_SetIpSrc(kCLOCK_Ftm0, kCLOCK_IpSrcFircAsync);

    /* Select the clock source for the LPI2C module as fast internal RC oscillator */
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_IpSrcFircAsync);
}

void BOARD_ACCEL_Reset(void)
{
    uint32_t i;
    gpio_pin_config_t pin_config;

    /* Reset sensor by reset pin*/
    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic  = 1;
    GPIO_PinInit(BOARD_ACCEL_RESET_GPIO, BOARD_ACCEL_RESET_PIN, &pin_config);
    GPIO_PinWrite(BOARD_ACCEL_RESET_GPIO, BOARD_ACCEL_RESET_PIN, 1);
    /* Delay to ensure reliable sensor reset */
    for (i = 0; i < SystemCoreClock / 1000U; i++)
    {
        __NOP();
    }
    GPIO_PinWrite(BOARD_ACCEL_RESET_GPIO, BOARD_ACCEL_RESET_PIN, 0);

    /* Delay to wait sensor stable after reset */
    for (i = 0; i < SystemCoreClock / 1000U; i++)
    {
        __NOP();
    }
}
/*${function:end}*/
