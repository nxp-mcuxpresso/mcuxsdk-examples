/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "fsl_smc.h"
#include "fsl_lpi2c.h"
#include "peripherals.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
/* Initialize debug console. */
void DEMO_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;

    CLOCK_SetIpSrc(kCLOCK_Lpuart1, kCLOCK_IpSrcSircAsync);
    uartClkSrcFreq = CLOCK_GetIpFreq(kCLOCK_Lpuart1);

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

void BOARD_InitHardware(void)
{
    gpio_pin_config_t pin_config;
    uint32_t i;

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    DEMO_InitDebugConsole();

    /* Reset sensor by reset pin*/
    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic  = 1;
    GPIO_PinInit(ACCEL_RESET_GPIO, ACCEL_RESET_PIN, &pin_config);
    GPIO_PinWrite(ACCEL_RESET_GPIO, ACCEL_RESET_PIN, 1);
    /* Delay to ensure reliable sensor reset */
    for (i = 0; i < SystemCoreClock / 1000U; i++)
    {
        __NOP();
    }
    GPIO_PinWrite(ACCEL_RESET_GPIO, ACCEL_RESET_PIN, 0);

    /* Delay to wait sensor stable after reset */
    for (i = 0; i < SystemCoreClock / 100U; i++)
    {
        __NOP();
    }

    BOARD_InitBootPeripherals();
}
/*${function:end}*/
