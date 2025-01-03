/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_port.h"
#include "fsl_adapter_gpio.h"
#include "usb_pd_config.h"
#include "usb_pd.h"
#include "pd_board_config.h"
/*${header:end}*/

/*${function:start}*/
#define I2C_RELEASE_BUS_COUNT 100U
#define PD_I2C_SCL            (1U)
#define PD_I2C_SDA            (2U)
#define PD_I2C_PORT           (2)

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockHSRUN();
    CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcSircAsync);
    CLOCK_SetIpSrc(kCLOCK_Lpi2c1, kCLOCK_IpSrcFircAsync);
    BOARD_InitDebugConsole();
    LPI2C1_InitPins();
}

uint32_t HW_TimerGetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpit0);
}

uint32_t HW_I2CGetFreq(uint8_t instance)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpi2c1);
}

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

void HW_I2CReleaseBus(void)
{
    hal_gpio_pin_config_t config;
    GPIO_HANDLE_DEFINE(pdI2cSclGpio);
    GPIO_HANDLE_DEFINE(pdI2cSdaGpio);
    uint8_t i                        = 0;
    port_pin_config_t i2c_pin_config = {0};
    PORT_Type *portList[]            = PORT_BASE_PTRS;

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux        = kPORT_MuxAsGpio;
    PORT_SetPinConfig(portList[PD_I2C_PORT], PD_I2C_SCL, &i2c_pin_config);
    PORT_SetPinConfig(portList[PD_I2C_PORT], PD_I2C_SDA, &i2c_pin_config);

    /* Init I2C GPIO */
    config.direction = kHAL_GpioDirectionOut;
    config.port      = PD_I2C_PORT;
    config.pin       = PD_I2C_SCL;
    config.level     = 1;
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
    LPI2C1_InitPins();
}

/* ${function:end} */
