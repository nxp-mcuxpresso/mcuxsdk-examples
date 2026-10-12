/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void EXAMPLE_EnableDeepSleepIRQ(void)
{
    EnableIRQ(OS_EVENT_IRQn);
}

void EXAMPLE_EnterDeepSleep(void)
{
}

void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* The OS timer comes out of boot in reset with its functional clock off, and the driver does
       not release the reset on this device, so do it here before the example uses the timer. */
    (void)CLOCK_EnableClock(kCLOCK_OsTimerPeriph);
    RESET_ClearPeripheralReset(kOSTIMER_CGU_RST_SHIFT_RSTn);
}
/*${function:end}*/
