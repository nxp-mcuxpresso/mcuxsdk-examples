/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_adapter_gpio.h"

#include "usb_pd_config.h"
#include "usb_pd.h"
#include "pd_board_config.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

void HW_I2CReleaseBus(void);

/* ${function:start} */
#define I2C_RELEASE_BUS_COUNT 1000U
#define PD_I2C_SCL            (25U)
#define PD_I2C_SDA            (24U)
#define PD_I2C_PORT           (0U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpi2c1_fclk)
/* Get source clock for LPTMR driver */
#define USBPD_LPTMR_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_WAKE_lptmr0_fclk)

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitOM13790HOST_GPIOPins();
    HW_I2CReleaseBus();
    NVIC_SetPriority(HSP_LPUART0_IRQn, 5);
}

uint32_t HW_TimerGetFreq(void)
{
    return USBPD_LPTMR_CLK_FREQ;
}

uint32_t HW_I2CGetFreq(uint8_t instance)
{
    return LPI2C_CLOCK_FREQUENCY;
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

    uint8_t i = 0;

    /* Config pin mux as gpio */
    CLOCK_EnableClock(kCLOCK_MAIN_iomuxc);

    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO2_25_HSP_GPIO0_GPIO25, 0x00U);
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO2_24_HSP_GPIO0_GPIO24, 0x00U);

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
    BOARD_InitOM13790HOST_I2CPins();
}

/*${function:end}*/

/*!
** @}
*/
