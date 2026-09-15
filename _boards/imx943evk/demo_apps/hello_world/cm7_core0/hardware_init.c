/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"

#if defined(MUX_REGION_ADDR) && defined(MUX_CORE_INDEX)
#include "fsl_debug_console.h"
#endif
/*${header:end}*/

/*${function:start}*/
#if defined(MUX_REGION_ADDR) && defined(MUX_CORE_INDEX)
#ifndef MUX_BAUDRATE
#define MUX_BAUDRATE 115200U
#endif
#endif

void BOARD_InitHardware(void)
{
#if defined(MUX_REGION_ADDR) && defined(MUX_CORE_INDEX)
    /* serial_mux enabled: console client. No local UART is owned here, only
     * system/clock bring-up. Console traffic travels through the
     * shared-memory slots polled by the hub (cm33_core1) and out on the
     * single physical UART. The stock hello_world.c runs UNMODIFIED: after
     * this PRINTF/GETCHAR flow through the hub on this core's logical
     * channel. */
    SystemPlatformInit();
    BOARD_BootClockRUN();
    BOARD_ConfigMPU();

    /* The muxuart HAL UART adapter is the console's UART, so the ordinary
     * debug_console_lite bring-up is all that is needed. The HAL UART
     * "instance" is this core's 0-based index (its mux channel) and the
     * source clock is irrelevant because the hub owns the physical wire. */
    DbgConsole_Init((uint8_t)MUX_CORE_INDEX, MUX_BAUDRATE, kSerialPort_Uart, 0U);
#else
    /* Default: standard hello_world on this core's own physical debug UART. */
    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();
#endif
}
/*${function:end}*/
