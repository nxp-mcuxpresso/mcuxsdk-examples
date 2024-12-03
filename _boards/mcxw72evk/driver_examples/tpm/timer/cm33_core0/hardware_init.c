/*
 * Copyright 2023 NXP
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

    /* TPM 0 Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_Tpm0);

    /* Set the source for the LPIT module */
    CLOCK_SetIpSrc(kCLOCK_Tpm0, kCLOCK_IpSrcFro6M);
}
/*${function:end}*/
