/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Enable the 32 kHz output of the RTC oscillator */
    SYSCON->RTCOSCCTRL |= SYSCON_RTCOSCCTRL_EN_MASK;
    /* Enable the interrupt of FLEXCOMM0 for wake-up from deep-sleep mode */
    SYSCON->STARTERSET[0] |= SYSCON_STARTER_FLEXCOMM0_MASK;
}

void BOARD_SwitchBackClockSrc()
{
    BOARD_BootClockPLL220M();
}
/*${function:end}*/
