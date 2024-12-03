/*
 * Copyright  2018 NXP
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
    /* Enable clock of uart0. */
    CLOCK_EnableClock(kCLOCK_Uart0);
    /* Ser DIV of uart0. */
    CLOCK_SetClkDivider(kCLOCK_DivUsartClk, 1U);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Connect trigger sources to PINT */
    SYSCON_AttachSignal(SYSCON, kPINT_PatternMatchInp0Src, kSYSCON_GpioPort0Pin4ToPintsel);
    SYSCON_AttachSignal(SYSCON, kPINT_PatternMatchInp1Src, kSYSCON_GpioPort0Pin12ToPintsel);
}
/*${function:end}*/
