/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_power.h"
#include "fsl_gpio.h"
#include "fsl_pint.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${function:start}*/
void EXAMPLE_PinIntHandler(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    APP_ACCEL_IntHandler();
}

void EXAMPLE_InitWakeupPin(void)
{
    gpio_pin_config_t gpioPinConfigStruct;
    gpio_interrupt_config_t config = {kGPIO_PinIntEnableEdge, kGPIO_PinIntEnableLowOrFall};

    /* Set SW pin as GPIO input. */
    gpioPinConfigStruct.pinDirection = kGPIO_DigitalInput;
    GPIO_PinInit(EXAMPLE_SENSOR_INT_GPIO, EXAMPLE_SENSOR_INT_PORT, EXAMPLE_SENSOR_INT_PIN, &gpioPinConfigStruct);

    /* Enable GPIO pin interrupt */
    GPIO_SetPinInterruptConfig(EXAMPLE_SENSOR_INT_GPIO, EXAMPLE_SENSOR_INT_PORT, EXAMPLE_SENSOR_INT_PIN, &config);
    GPIO_PinEnableInterrupt(EXAMPLE_SENSOR_INT_GPIO, EXAMPLE_SENSOR_INT_PORT, EXAMPLE_SENSOR_INT_PIN, 0);

    EnableIRQ(EXAMPLE_INT_IRQn);

    EnableDeepSleepIRQ(EXAMPLE_INT_IRQn);
}

void EXAMPLE_EnterDeepSleep(void)
{
    /* Enter deep sleep mode by using power API. */
    POWER_EnterDeepSleep(EXAMPLE_EXCLUDE_FROM_DEEPSLEEP);
}

void BOARD_InitHardware(void)
{
    /* Use 16 MHz clock for the FLEXCOMM2 */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM2);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
