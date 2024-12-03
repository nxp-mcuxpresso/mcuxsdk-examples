/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach 12 MHz clock to FLEXCOMM7 (I2C master) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom7Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom7Clk, 1u, true);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM7);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC7_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_BootClockPLL150M();
    //   BOARD_I2C_ReleaseBus();
    BOARD_InitDebugConsole();
}

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;

    IOCON->PIO[1][21] &= 0xFFF1;
    IOCON->PIO[1][30] &= 0xFFF1;

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(GPIO, 1U, 21U, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(GPIO, 1U, 30U, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(GPIO, 1U, 21U, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(GPIO, 1U, 30U, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(GPIO, 1U, 30U, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(GPIO, 1U, 21U, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(GPIO, 1U, 30U, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(GPIO, 1U, 21U, 1U);
    i2c_release_bus_delay();
}
/*${function:end}*/
