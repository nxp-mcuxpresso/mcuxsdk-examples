/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_adapter_gpio.h"

#include "usb_pd_config.h"
#include "usb_pd.h"
#include "pd_board_config.h"
#include "pin_mux.h"
#include "peripherals.h"
/*${header:end}*/

void HW_I2CReleaseBus(void);
uint32_t HW_TimerGetFreq(void);
uint32_t HW_I2CGetFreq(uint8_t instance);

/* ${function:start} */
#define I2C_RELEASE_BUS_COUNT 100U
#define PD_I2C_SCL            (15U)
#define PD_I2C_SDA            (14U)
#define PD_I2C_PORT           (2)

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    BOARD_InitDebugConsole();
    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_ERR_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TX_IDLE_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);

    HW_I2CReleaseBus();
    I2C0_InitPins();
    SetIRQBasePriority(0);
}

uint32_t HW_TimerGetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_BusClk);
}

uint32_t HW_I2CGetFreq(uint8_t instance)
{
    return CLOCK_GetIpClkSrcFreq(kCLOCK_I2C0);
}

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        asm(NOP);
    }
}

void HW_I2CReleaseBus(void)
{
    hal_gpio_pin_config_t config;

    GPIO_HANDLE_DEFINE(pdI2cSclGpio);

    GPIO_HANDLE_DEFINE(pdI2cSdaGpio);

    uint8_t i = 0;

    /* Config pin mux as gpio */
    I2C0_DeinitPins();

    /* Init I2C GPIO */

    config.direction = kHAL_GpioDirectionOut;

    config.port = PD_I2C_PORT;

    config.pin = PD_I2C_SCL;

    config.level = 1;

    HAL_GpioInit((hal_gpio_handle_t)pdI2cSclGpio, &config);

    config.pin = PD_I2C_SDA;

    HAL_GpioInit((hal_gpio_handle_t)pdI2cSdaGpio, &config);

    i2c_release_bus_delay();

    /* Drive SDA low first to simulate a start */
    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSdaGpio, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSclGpio, 0U);
        i2c_release_bus_delay();

        HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSdaGpio, 1U);
        i2c_release_bus_delay();

        HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSclGpio, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSclGpio, 0U);
    i2c_release_bus_delay();

    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSdaGpio, 0U);
    i2c_release_bus_delay();

    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSclGpio, 1U);
    i2c_release_bus_delay();

    HAL_GpioSetOutput((hal_gpio_handle_t)pdI2cSdaGpio, 1U);
    i2c_release_bus_delay();
    /* De-init I2C GPIO */

    HAL_GpioDeinit((hal_gpio_handle_t)pdI2cSclGpio);

    HAL_GpioDeinit((hal_gpio_handle_t)pdI2cSdaGpio);

    /* re-configure pin mux as i2c */
    I2C0_InitPins();
}

/*${function:end}*/

/*!
** @}
*/
