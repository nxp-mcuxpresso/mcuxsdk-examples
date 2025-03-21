/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include "app.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_EnableClock(kCLOCK_GateLPUART2);
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP1);
    CLOCK_EnableClock(kCLOCK_GateOSTIMER0);
    CLOCK_AttachClk(kCLK_1M_to_OSTIMER0);

    /* Release peripheral RESET */
    RESET_PeripheralReset(kOSTIMER0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kPORT1_RST_SHIFT_RSTn);

    BOARD_InitPins();
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
