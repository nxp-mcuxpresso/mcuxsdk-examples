/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_PctlB);
    CLOCK_EnableClock(kCLOCK_Rgpio2p0);
    CLOCK_SetIpSrc(kCLOCK_Lpi2c3, kCLOCK_IpSrcSircAsync);
}

void BOARD_I2C_ReleaseBusDelay(void)
{
    uint32_t i = 0U;
    for (i = 0U; i < DEMO_PF1550_LPI2C_DELAY; i++)
    {
        __NOP();
    }
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0U;
    gpio_pin_config_t pin_config;

    /* Initialize PTB12/PTB13 as GPIO */
    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic  = 1U;
    IOMUXC_SetPinMux(IOMUXC_PTB12_PTB12, 0U);
    IOMUXC_SetPinMux(IOMUXC_PTB13_PTB13, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PTB12_PTB12, IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PTB13_PTB13, IOMUXC0_SW_MUX_CTL_PAD_OBE_MASK);
    CLOCK_EnableClock(kCLOCK_Rgpio2p0);
    GPIO_PinInit(DEMO_PF1550_LPI2C_SCL_GPIO, DEMO_PF1550_LPI2C_SCL_PIN, &pin_config);
    GPIO_PinInit(DEMO_PF1550_LPI2C_SDA_GPIO, DEMO_PF1550_LPI2C_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(DEMO_PF1550_LPI2C_SDA_GPIO, DEMO_PF1550_LPI2C_SDA_PIN, 0U);
    BOARD_I2C_ReleaseBusDelay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0U; i < 9U; i++)
    {
        GPIO_PinWrite(DEMO_PF1550_LPI2C_SCL_GPIO, DEMO_PF1550_LPI2C_SCL_PIN, 0U);
        BOARD_I2C_ReleaseBusDelay();

        GPIO_PinWrite(DEMO_PF1550_LPI2C_SDA_GPIO, DEMO_PF1550_LPI2C_SDA_PIN, 1U);
        BOARD_I2C_ReleaseBusDelay();

        GPIO_PinWrite(DEMO_PF1550_LPI2C_SCL_GPIO, DEMO_PF1550_LPI2C_SCL_PIN, 1U);
        BOARD_I2C_ReleaseBusDelay();
        BOARD_I2C_ReleaseBusDelay();
    }

    /* Send stop */
    GPIO_PinWrite(DEMO_PF1550_LPI2C_SCL_GPIO, DEMO_PF1550_LPI2C_SCL_PIN, 0U);
    BOARD_I2C_ReleaseBusDelay();

    GPIO_PinWrite(DEMO_PF1550_LPI2C_SDA_GPIO, DEMO_PF1550_LPI2C_SDA_PIN, 0U);
    BOARD_I2C_ReleaseBusDelay();

    GPIO_PinWrite(DEMO_PF1550_LPI2C_SCL_GPIO, DEMO_PF1550_LPI2C_SCL_PIN, 1U);
    BOARD_I2C_ReleaseBusDelay();

    GPIO_PinWrite(DEMO_PF1550_LPI2C_SDA_GPIO, DEMO_PF1550_LPI2C_SDA_PIN, 1U);
    BOARD_I2C_ReleaseBusDelay();
}
/*${function:end}*/
