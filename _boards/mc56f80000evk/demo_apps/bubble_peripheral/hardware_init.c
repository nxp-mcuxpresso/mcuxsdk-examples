/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "peripherals.h"
#include "fsl_gpio.h"
/*${header:end}*/

/*${function:start}*/
static void I2C_Delay(void)
{
    uint32_t i;
    for (i = 0; i < 100; i++)
    {
        __NOP();
    }
}

static void Accel_I2C_Release(void)
{
    while (1)
    {
        GPIO_PinSet(ACCEL_SCL_PORT, ACCEL_SCL_PIN);
        I2C_Delay();
        if (1U == GPIO_PinRead(ACCEL_SDA_PORT, ACCEL_SDA_PIN))
        {
            GPIO_PinSet(ACCEL_SDA_PORT, ACCEL_SDA_PIN);
            GPIO_PinSetDirection(ACCEL_SDA_PORT, ACCEL_SDA_PIN, kGPIO_DigitalOutput);
            I2C_Delay();
            GPIO_PinClear(ACCEL_SDA_PORT, ACCEL_SDA_PIN);
            I2C_Delay();
            GPIO_PinSet(ACCEL_SDA_PORT, ACCEL_SDA_PIN);
            I2C_Delay();
            break;
        }
        GPIO_PinClear(ACCEL_SCL_PORT, ACCEL_SCL_PIN);
        I2C_Delay();
    }
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    Accel_I2C_Release();
    Accel_InitPins();

    BOARD_InitBootPeripherals();

    BOARD_InitDebugConsole();

    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_ERR_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TRX_IDLE_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    SetIRQBasePriority(0);
}
/*${function:end}*/
