/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kOSTIMER0_RST_SHIFT_RSTn);

    /* Attach peripheral clock */
    CLOCK_SetupFRO16KClocking(kCLKE_16K_SYSTEM | kCLKE_16K_COREMAIN);
    CLOCK_AttachClk(kCLK_16K_to_OSTIMER);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/* Enter deep sleep mode. */
void EXAMPLE_EnterDeepSleep()
{
    /* TODO: enter deep sleep */
}
/* Enable OSTIMER IRQ under deep mode */
void EXAMPLE_EnableDeepSleepIRQ(void)
{
    EnableIRQ(OS_EVENT_IRQn);
}
/*${function:end}*/
