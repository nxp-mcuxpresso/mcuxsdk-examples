/*
 * Copyright 2020, 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_soc_src.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();

    /*
     * To improve performance, some LVGL code is copied to cacheable RAM region,
     * like OCRAM. So clean DCache then invalidate ICache.
     * M4 doesn't need it, because BOARD_ConfigMPU has done it.
     */
#if (__CORTEX_M == 7)
    SCB_CleanDCache();
    SCB_InvalidateICache();
    __DSB();
    __ISB();
#endif

    BOARD_BootClockRUN();

    /*
     * Reset the displaymix, otherwise during debugging, the
     * debugger may not reset the display, then the behavior
     * is not right.
     */
    SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);

    BOARD_InitLpuartPins();
    BOARD_InitMipiPanelPins();
    BOARD_MIPIPanelTouch_I2C_Init();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
