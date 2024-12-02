/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_common.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_ACCEL_Reset();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();
    CLOCK_SetLpuart0Clock(1U);
}

void BOARD_ACCEL_Reset(void)
{
    gpio_pin_config_t pin_config;

    /* Reset sensor by reset pin*/
    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic  = 1;
    GPIO_PinInit(BOARD_ACCEL_RESET_GPIO, BOARD_ACCEL_RESET_PIN, &pin_config);
    GPIO_PinWrite(BOARD_ACCEL_RESET_GPIO, BOARD_ACCEL_RESET_PIN, 1);

    /* Delay to ensure reliable sensor reset */
    SDK_DelayAtLeastUs(8000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    GPIO_PinWrite(BOARD_ACCEL_RESET_GPIO, BOARD_ACCEL_RESET_PIN, 0);

    /* Delay to wait sensor stable after reset */
    SDK_DelayAtLeastUs(8000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

uint32_t LPUART0_GetFreq(void)
{
    return CLOCK_GetFreq(SYS_CLK);
}
/*${function:end}*/
