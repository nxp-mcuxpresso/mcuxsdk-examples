/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    /* Select the clock source for the TPM counter as  slow internal RC oscillator */
    CLOCK_SetIpSrc(kCLOCK_Tpm0, kCLOCK_IpSrcSircAsync);
}
/*${function:end}*/
