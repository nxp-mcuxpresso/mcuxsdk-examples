/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "app.h"
#include "lvgl_support.h"
/*${header:end}*/

/*${macro:start}*/
#define I2C_RELEASE_SDA_PORT  PORT1
#define I2C_RELEASE_SCL_PORT  PORT1
#define I2C_RELEASE_SDA_GPIO  GPIO1
#define I2C_RELEASE_SDA_PIN   8U
#define I2C_RELEASE_SCL_GPIO  GPIO1
#define I2C_RELEASE_SCL_PIN   9U
#define I2C_RELEASE_BUS_COUNT 100U
/*${macro:end}*/

/*${function:start}*/
void BOARD_I2C_ReleaseBus(void);

void BOARD_InitHardware(void)
{
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivFLEXIO0, 1u);
    CLOCK_AttachClk(kFRO_HF_to_FLEXIO0);

    CLOCK_SetClockDiv(kCLOCK_DivLPI2C2, 1u);
    CLOCK_AttachClk(kFRO12M_to_LPI2C2);

    CLOCK_EnableClock(kCLOCK_GateGPIO0);
    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    CLOCK_EnableClock(kCLOCK_GateGPIO2);
    CLOCK_EnableClock(kCLOCK_GateGPIO3);
    CLOCK_EnableClock(kCLOCK_GateGPIO4);

    CLOCK_EnableClock(kCLOCK_GatePORT0);
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    CLOCK_EnableClock(kCLOCK_GatePORT4);
    CLOCK_EnableClock(kCLOCK_GateFLEXIO0);

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kFLEXIO0_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kGPIO4_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT4_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT0_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);

    BOARD_I2C_ReleaseBus();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

static void i2c_release_bus_delay(void)
{
    SDK_DelayAtLeastUs(100U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config = {0};

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux        = kPORT_MuxAsGpio;

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic  = 1U;
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}

void BOARD_LCD_INT_IRQHandler(void)
{
    uint32_t flags = GPIO_GpioGetInterruptFlags(BOARD_LCD_INT_GPIO);

    GPIO_GpioClearInterruptFlags(BOARD_LCD_INT_GPIO, flags);

    if ((flags & (1UL << BOARD_LCD_INT_PIN)) != 0U)
    {
        BOARD_TouchIntHandler();
    }

    SDK_ISR_EXIT_BARRIER;
}
/*${function:end}*/
