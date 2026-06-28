/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_soc_src.h"
#include "board.h"
#include "lcdifv2_support.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
static void BOARD_ResetDisplayMix(void)
{
    /*
     * Reset the displaymix, otherwise during debugging, the
     * debugger may not reset the display, then the behavior
     * is not right.
     */
    SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);
    while (kSRC_SliceResetInProcess == SRC_GetSliceResetState(SRC, kSRC_DisplaySlice))
    {
    }
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();
    BOARD_ResetDisplayMix();
    BOARD_InitBootPins();
    BOARD_Init6524Pins();
#if (USE_MIPI_PANEL == MIPI_PANEL_RASPI_7INCH)
    BOARD_InitI2CPins();
#endif
    BOARD_InitDebugConsole();
    BOARD_InitLcdifClock();
}
/*${function:end}*/
