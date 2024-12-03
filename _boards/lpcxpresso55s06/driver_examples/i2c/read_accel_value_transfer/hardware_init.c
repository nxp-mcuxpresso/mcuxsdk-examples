/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "app.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitDebugAccelConsole(void)
{
    /* attach 12 MHz clock to FLEXCOMM1 (debug console) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM1);

    RESET_ClearPeripheralReset(kFC1_RST_SHIFT_RSTn);

    uint32_t uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;

    DbgConsole_Init(1U, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM1 (debug console) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM1);

    /* attach 12 MHz clock to FLEXCOMM0 (I2C master) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_BootClockFROHF96M();
    //   BOARD_I2C_ReleaseBus();
    BOARD_InitDebugAccelConsole();
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

    IOCON->PIO[0][24] &= 0xFFF1;
    IOCON->PIO[0][25] &= 0xFFF1;

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(GPIO, 0U, 24U, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(GPIO, 0U, 25U, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(GPIO, 0U, 24U, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(GPIO, 0U, 25U, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(GPIO, 0U, 25U, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(GPIO, 0U, 24U, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(GPIO, 0U, 25U, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(GPIO, 0U, 24U, 1U);
    i2c_release_bus_delay();
}
/*${function:end}*/
