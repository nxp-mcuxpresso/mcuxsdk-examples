/*
 * Copyright 2026 NXP
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
#include "fsl_smartdma.h"
#include "fsl_inputmux_connections.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${macro:start}*/
#define I2C_RELEASE_SDA_PORT  PORT3
#define I2C_RELEASE_SCL_PORT  PORT3
#define I2C_RELEASE_SDA_GPIO  GPIO3
#define I2C_RELEASE_SDA_PIN   20U
#define I2C_RELEASE_SCL_GPIO  GPIO3
#define I2C_RELEASE_SCL_PIN   21U
#define I2C_RELEASE_BUS_COUNT 100U
/*${macro:end}*/

/*${function:start}*/
void BOARD_I2C_ReleaseBus(void);

static void BOARD_InitSmartDMA(void)
{
    RESET_ClearPeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);

    INPUTMUX_Init(INPUTMUX0);
    INPUTMUX_AttachSignal(INPUTMUX0, 0, kINPUTMUX_Flexio0irqToSmartdmatrig);

    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX0);

    SMARTDMA_InitWithoutFirmware();

    NVIC_EnableIRQ(SMARTDMA_IRQn);
    NVIC_SetPriority(SMARTDMA_IRQn, 3);
 }

void BOARD_InitHardware(void)
{
    CLOCK_EnableClock(kCLOCK_GateGPIO3);
    CLOCK_EnableClock(kCLOCK_GatePORT3);

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kGPIO3_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT3_RST_SHIFT_RSTn);

    BOARD_I2C_ReleaseBus();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitFLEXIO_MCULCDPins();
    BOARD_InitFlexioMculcdSmartdmaTrigger();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Init smartdma. */
    BOARD_InitSmartDMA();

    /* Attach peripheral clock */
    CLOCK_AttachClk(kFRO_HF_to_FLEXIO0);
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPI2C3);

    CLOCK_SetClockDiv(kCLOCK_DivFLEXIO0, 1u);
    CLOCK_SetClockDiv(kCLOCK_DivLPI2C3, 1u);
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
