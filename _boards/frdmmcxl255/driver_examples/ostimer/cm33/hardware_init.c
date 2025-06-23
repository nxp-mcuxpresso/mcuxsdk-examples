/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_power.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{   
    RESET_ReleasePeripheralReset(kOSTIMER0_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_GateOSTIMER0);
    
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

/* Enter deep sleep mode. */
void EXAMPLE_EnterDeepSleep()
{
    while (OSTIMER0->OSEVENT_CTRL & OSTIMER_OSEVENT_CTRL_MATCH_WR_RDY_MASK);

    power_ds_config_t dsConfig;

    Power_EnterDeepSleep(&dsConfig);
}
/* Enable OSTIMER IRQ under deep mode */
void EXAMPLE_EnableDeepSleepIRQ(void)
{
    EnableIRQ(EXAMPLE_OSTIMER_IRQn);
}
/*${function:end}*/