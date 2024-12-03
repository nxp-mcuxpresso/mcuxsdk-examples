/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"

#include "fsl_syscon.h"
#include "fsl_pint.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Select the main clock as source clock of USART0 (debug console) */
    CLOCK_Select(BOARD_DEBUG_USART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_BootClockFRO48M();
    BOARD_InitDebugConsole();

    /* Connect trigger sources to PINT */
    SYSCON_AttachSignal(SYSCON, kPINT_PatternMatchInp0Src, kSYSCON_GpioPort0Pin12ToPintsel);
    SYSCON_AttachSignal(SYSCON, kPINT_PatternMatchInp1Src, kSYSCON_GpioPort0Pin4ToPintsel);
}
/*${function:end}*/
