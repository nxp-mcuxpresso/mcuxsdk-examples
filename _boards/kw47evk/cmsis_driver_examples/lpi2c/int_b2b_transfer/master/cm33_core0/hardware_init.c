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
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Clock setting for LPI2C*/
    CLOCK_SetIpSrc(LPI2C_MASTER_CLOCK, kCLOCK_IpSrcFro192M);
    CLOCK_SetIpSrcDiv(LPI2C_MASTER_CLOCK, kSCG_SysClkDivBy16);
}

uint32_t LPI2C1_GetFreq(void)
{
    return LPI2C_MASTER_CLOCK_FREQUENCY;
}
/*${function:end}*/
