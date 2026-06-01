/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "app.h"
#include "fsl_irqsteer.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    SystemPlatformInit();
    BOARD_ConfigMPU();
    BOARD_InitDebugConsolePins();
    BOARD_InitDebugConsole();

    IRQSTEER_Init(IRQSTEERM7_INST);
    IRQSTEER_EnableInterrupt(IRQSTEERM7_INST, EXAMPLE_MSGINTR_IRQN);
}
/*${function:end}*/
