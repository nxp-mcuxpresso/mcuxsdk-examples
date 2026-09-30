/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
#include "v2x_fce.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();
}

/* V2X-FH MU RX interrupt handler for this core. The vector name is
 * SoC/core specific; it simply forwards to the portable driver hook so
 * the v2x_fce interrupt-driven completion path can run here. */
void Reserved400_IRQHandler(void)
{
    V2X_FCE_HandleIRQ();
}
/*${function:end}*/
