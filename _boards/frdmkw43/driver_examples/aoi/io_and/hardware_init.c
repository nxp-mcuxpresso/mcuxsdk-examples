/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_trgmux.h"
/*${header:end}*/

/*${function:start}*/
void IO_Configuration(void)
{
    TRGMUX_SetTriggerSource(TRGMUX_0, kTRGMUX_Trgmux0Aoi0, kTRGMUX_TriggerInput0, kTRGMUX_SourceTrgmux0Input0);
    TRGMUX_SetTriggerSource(TRGMUX_0, kTRGMUX_Trgmux0Aoi0, kTRGMUX_TriggerInput1, kTRGMUX_SourceTrgmux0Input1);

    TRGMUX_SetTriggerSource(TRGMUX_0, kTRGMUX_Trgmux0Output0, kTRGMUX_TriggerInput0, kTRGMUX_SourceAoiOut0);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Trgmux0);
}
/*${function:end}*/
