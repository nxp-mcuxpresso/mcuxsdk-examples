/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_fmeas.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* enable clock for INPUTMUX */
    CLOCK_EnableClock(kCLOCK_InputMux);

    /* enable clock for IOCON */
    CLOCK_EnableClock(kCLOCK_Iocon);

    /* enable clock for watchdog */
    CLOCK_EnableClock(kCLOCK_Wwdt);

    /* Set watchdog oscilator to freq 1 MHz / 2 */
    /* TODO use API once available */
    SYSCON->WDTOSCCTRL = SYSCON_WDTOSCCTRL_DIVSEL(0x00) | SYSCON_WDTOSCCTRL_FREQSEL(0x05);
    /* Enable the bus clk for RTC */
    CLOCK_EnableClock(kCLOCK_Rtc);
    /* RTC oscillator power-on*/
    RTC->CTRL &= ~RTC_CTRL_RTC_OSC_PD_MASK;

    /* power up watchdog */
    POWER_DisablePD(kPDRUNCFG_PD_WDT_OSC);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
