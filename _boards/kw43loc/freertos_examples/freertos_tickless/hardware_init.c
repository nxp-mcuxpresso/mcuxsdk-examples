/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "FreeRTOS.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitButtonsPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(kCLOCK_Lptmr0, kCLOCK_IpSrcFro6M);
    CLOCK_SetIpSrcDiv(kCLOCK_Lptmr0, 1U);
    CLOCK_EnableClock(kCLOCK_Lptmr0);
}
/*${function:end}*/
