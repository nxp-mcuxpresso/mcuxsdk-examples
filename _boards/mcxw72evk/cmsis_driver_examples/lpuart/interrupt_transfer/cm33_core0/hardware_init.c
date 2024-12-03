/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();

    CLOCK_SetIpSrc(kCLOCK_Lpuart1, kCLOCK_IpSrcFro6M);
}

uint32_t LPUART1_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpuart1);
}
/*${function:end}*/
