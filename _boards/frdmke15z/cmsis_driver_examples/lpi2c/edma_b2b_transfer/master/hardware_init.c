/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* ${header:start} */
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
/* ${header:end} */

/* ${function:start} */
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_IpSrcFircAsync);
}
uint32_t LPI2C0_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpi2c0);
}
/* ${function:end} */
