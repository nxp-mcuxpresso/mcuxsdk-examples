/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include <xtensa/config/core.h>
#include <xtensa/xos.h>
#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

/*${function:start}*/
static void XOS_INIT(void)
{
    xos_set_clock_freq(BOARD_XTAL_SYS_CLK_HZ);
    xos_start_system_timer(-1, 0);
}
void BOARD_InitHardware(void)
{
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* Note: need tell clock driver the frequency of OSC. */
    INPUTMUX_Init(INPUTMUX0);
    CLOCK_EnableClock(kCLOCK_InputMux);
    INPUTMUX_AttachSignal(INPUTMUX0, 1U, kINPUTMUX_Mu4AToDspInterrupt);
    BOARD_InitBootPins();
    /* Disable DSP cache for RPMsg-Lite shared memory. */
    xthal_set_region_attribute((void *)0x20000000, 6144U, XCHAL_CA_BYPASS, 0);
    XOS_INIT();
}
/*${function:end}*/
