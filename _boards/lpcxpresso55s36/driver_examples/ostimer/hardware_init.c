/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void EXAMPLE_EnableDeepSleepIRQ(void)
{
    PMC->OSEVENTTIMER |= PMC_OSEVENTTIMER_DPDWAKEUPENABLE_MASK;
}
void EXAMPLE_EnterDeepSleep(void)
{
    /* Enter deep sleep mode by using power API. */
    uint32_t exclude_from_pd[2];
    uint32_t wakeup_interrupts[4];
    exclude_from_pd[0]   = EXAMPLE_EXCLUDE_FROM_DEEPSLEEP;
    exclude_from_pd[1]   = 0u;
    wakeup_interrupts[0] = 0u;
    wakeup_interrupts[1] = EXAMPLE_WAKEUP_IRQ_SOURCE;
    wakeup_interrupts[2] = 0u;
    wakeup_interrupts[3] = 0u;
    POWER_EnterDeepSleep(exclude_from_pd, 0x0U, wakeup_interrupts, 0x0U);
}

void BOARD_InitHardware(void)
{
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockFRO12M();
    BOARD_InitDebugConsole();

    /* Clock setting for OS timer. choose xtal32k as the source clock of OS timer. */
    POWER_DisablePD(kPDRUNCFG_PD_XTAL32K);
    CLOCK_AttachClk(kXTAL32K_to_OSC32K);
    CLOCK_AttachClk(kOSC32K_to_OSTIMER);
}
/*${function:end}*/
