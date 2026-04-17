/*
 * Copyright 2026 NXP
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
    BOARD_InitBootPins();
    BOARD_BootClockRUN();

    CLOCK_SetIpSrc(kCLOCK_Lpuart1, kCLOCK_IpSrcFro192M);
    CLOCK_SetIpSrcDiv(kCLOCK_Lpuart1, 1U);
    CLOCK_EnableClock(kCLOCK_Fro_hf_div);
}

uint32_t LPUART1_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpuart1);
}
/*${function:end}*/
