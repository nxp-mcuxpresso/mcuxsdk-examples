/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_DebugConsolePins();
    BOARD_InitLeds();
    BOARD_InitButtons();
    BOARD_InitLinLpuart();
    /* Set LIN LPUART clock */
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcFro6M);
#if defined(TIMER_TPM) && TIMER_TPM
    /* Set Timer LPUART clock */
    CLOCK_SetIpSrc(DEMO_TPM_CLOCK, kCLOCK_IpSrcFro6M);
#endif
    BOARD_InitDebugConsole();
}
/*${function:end}*/
