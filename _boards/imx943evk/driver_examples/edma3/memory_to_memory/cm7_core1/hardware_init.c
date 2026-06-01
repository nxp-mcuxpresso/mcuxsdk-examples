/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    /* clang-format on */

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    /* DMA clocked by M33 clk, configurated by SM */
    //CLOCK_SetRate(&edma3Clk);
    //CLOCK_EnableClock(kCLOCK_Edma3);
}
/*${function:end}*/
