/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/* ${header:start} */
#include "app.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "board.h"
/* ${header:end} */

/* ${function:start} */
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_ACCEL_Reset();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();
    /* Select the clock source for the LPI2C module as fast internal RC oscillator */
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_IpSrcFircAsync);
    /* Select the clock source for the FTM counter as fast internal RC oscillator */
    CLOCK_SetIpSrc(kCLOCK_Ftm0, kCLOCK_IpSrcFircAsync);
    BOARD_InitBootPeripherals();
}

void BOARD_ACCEL_Reset(void)
{
    uint32_t i;

    /* Reset sensor by reset pin*/
    BOARD_Init_ACCEL_Reset();
    /* Delay to ensure reliable sensor reset */
    for (i = 0; i < SystemCoreClock / 1000U; i++)
    {
        __NOP();
    }
    GPIO_PinWrite(BOARD_ACCEL_RESET_GPIO, BOARD_ACCEL_RESET_PIN, 0);

    /* Delay to wait sensor stable after reset */
    for (i = 0; i < SystemCoreClock / 1000U; i++)
    {
        __NOP();
    }
}
/*${function:end}*/
