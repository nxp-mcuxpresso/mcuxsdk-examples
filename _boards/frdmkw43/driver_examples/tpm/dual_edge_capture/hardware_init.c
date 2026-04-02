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
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set the source for the TPM 0 module */
    CLOCK_SetIpSrc(kCLOCK_Tpm0, kCLOCK_IpSrcFro192M);
    CLOCK_SetIpSrcDiv(kCLOCK_Tpm0, 3U);
    CLOCK_EnableClock(kCLOCK_Fro_hf_div);
    CLOCK_EnableClock(kCLOCK_Tpm0);
}
/*${function:end}*/
