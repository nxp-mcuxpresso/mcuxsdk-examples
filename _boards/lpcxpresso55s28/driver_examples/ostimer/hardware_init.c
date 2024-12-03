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
    PMC->OSTIMERr |= PMC_OSTIMER_DPDWAKEUPENABLE_MASK;
}
void EXAMPLE_EnterDeepSleep(void)
{
    /* Enter deep sleep mode by using power API. */
    POWER_EnterDeepSleep(EXAMPLE_EXCLUDE_FROM_DEEPSLEEP, 0x0U, EXAMPLE_WAKEUP_IRQ_SOURCE, 0x0U);
}

void BOARD_InitHardware(void)
{
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Clock setting for OS timer. choose xtal32k as the source clock of OS timer. */
    POWER_EnablePD(kPDRUNCFG_PD_XTAL32K);
    PMC->RTCOSC32K |= PMC_RTCOSC32K_SEL_MASK;
    PMC->OSTIMERr &= ~PMC_OSTIMER_OSC32KPD_MASK;
}
/*${function:end}*/
