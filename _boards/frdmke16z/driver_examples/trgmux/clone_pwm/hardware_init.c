/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_trgmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Set the source for the LPIT module */
    CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcFircAsync);
}

void BOARD_ConfigTriggerSource(void)
{
    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_ExtOut0_3, kTRGMUX_TriggerInput0, kTRGMUX_SourceLpit0Ch0);
    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_ExtOut0_3, kTRGMUX_TriggerInput1, kTRGMUX_SourceLpit0Ch0);
    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_ExtOut0_3, kTRGMUX_TriggerInput2, kTRGMUX_SourceLpit0Ch0);
    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_ExtOut0_3, kTRGMUX_TriggerInput3, kTRGMUX_SourceLpit0Ch0);
}
/*${function:end}*/
