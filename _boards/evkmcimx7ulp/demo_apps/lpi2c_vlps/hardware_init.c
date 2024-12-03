/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_msmc.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/

/* Initialize debug console. */
void APP_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;

    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSircAsync);
    uartClkSrcFreq = CLOCK_GetIpFreq(kCLOCK_Lpuart0);

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_I2C_ConfigurePins();
    BOARD_BootClockRUN();
    APP_InitDebugConsole();

    PRINTF("Please hit any key to start demo\r\n");
    GETCHAR();
    /* Set LPI2C clock source, should work in stop mode */
    CLOCK_SetIpSrc(DEMO_ACCEL_I2C_CLOCK_NAME, DEMO_ACCEL_I2C_CLOCK_SOURCE);
    /* Select the SIRC 16M as LPIT clock, SIRC enabled in stop mode */
    CLOCK_SetIpSrc(kCLOCK_Lpit0, DEMO_LPIT_CLOCK_SOURCE);
}
/*${function:end}*/
