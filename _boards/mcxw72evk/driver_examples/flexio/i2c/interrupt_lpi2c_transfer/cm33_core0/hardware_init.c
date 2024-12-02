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
    BOARD_InitDebugConsole();

    /* Set MRCC FLEXIO0 selection */
    CLOCK_SetIpSrc(kCLOCK_Flexio0, kCLOCK_IpSrcFro192M);
    /* Set MRCC FLEXIO0 fraction divider */
    CLOCK_SetIpSrcDiv(kCLOCK_Flexio0, kSCG_SysClkDivBy16);
    /* Set MRCC LPI2C1 selection */
    CLOCK_SetIpSrc(kCLOCK_Lpi2c1, kCLOCK_IpSrcFro192M);
    /* Set MRCC LPI2C1 fraction divider */
    CLOCK_SetIpSrcDiv(kCLOCK_Lpi2c1, kSCG_SysClkDivBy16);
}
/*${function:end}*/
