/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${standard_header:start}*/
#include <stdbool.h>
/*${standard_header:end}*/
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_adapter_gpio.h"
#include "fsl_inputmux.h"
#include "fsl_pint.h"
#include "usb_pd_config.h"
#include "usb_pd.h"
#include "pd_board_config.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
#define PIO020_DIGIMODE_DIGITAL 0x01u /*!< Select Analog/Digital mode.: Digital mode. */
#define PIO020_FUNC_ALT0        0x00u /*!< Selects pin function.: Alternative connection 0. */
#define PIO021_DIGIMODE_DIGITAL 0x01u /*!< Select Analog/Digital mode.: Digital mode. */
#define PIO021_FUNC_ALT0        0x00u /*!< Selects pin function.: Alternative connection 0. */

#define I2C_RELEASE_BUS_COUNT 100U
#define PD_I2C_SCL            (20U)
#define PD_I2C_SDA            (21U)
#define PD_I2C_PORT           (1)

void BOARD_InitHardware(void)
{
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    /* attach 12 MHz clock to FLEXCOMM4 (I2C master) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);
    CLOCK_EnableClock(kCLOCK_Pint);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio1);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC4_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();
    NVIC_SetPriority(BOARD_UART_IRQ, 5);
    I2C4_InitPins();
}

uint32_t HW_TimerGetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_BusClk);
}

uint32_t HW_I2CGetFreq(uint8_t instance)
{
    return CLOCK_GetFlexCommClkFreq(4U);
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
    CLOCK_EnableClock(kCLOCK_Iocon);
    IOCON->PIO[PD_I2C_PORT][PD_I2C_SDA] =
        ((IOCON->PIO[PD_I2C_PORT][PD_I2C_SDA] & (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK))) |
         IOCON_PIO_FUNC(PIO021_FUNC_ALT0) | IOCON_PIO_DIGIMODE(PIO021_DIGIMODE_DIGITAL));
    IOCON->PIO[PD_I2C_PORT][PD_I2C_SCL] =
        ((IOCON->PIO[PD_I2C_PORT][PD_I2C_SCL] & (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK))) |
         IOCON_PIO_FUNC(PIO020_FUNC_ALT0) | IOCON_PIO_DIGIMODE(PIO020_DIGIMODE_DIGITAL));

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
    I2C4_InitPins();
}

/*${function:end}*/

/*!
** @}
*/
