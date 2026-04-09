/*
 * Copyright 2026 NXP
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
    BOARD_InitBootPins();
    BOARD_InitACCELPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(LPI2C_MASTER_CLOCK, kCLOCK_IpSrcFro192M);
    CLOCK_SetIpSrcDiv(LPI2C_MASTER_CLOCK, 1U);
    CLOCK_EnableClock(kCLOCK_Fro_hf_div);
}
/*${function:end}*/
