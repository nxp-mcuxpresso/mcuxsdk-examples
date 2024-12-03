/*
 * Copyright 2023 NXP
 * All rights reserved.
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
    /* Attach 12 MHz clock to LPUART (debug console) */
    CLOCK_SetClockDiv(kCLOCK_DivLPUART2, 1u);
    CLOCK_AttachClk(kFRO12M_to_LPUART2);

    BOARD_InitPins();
    BOARD_InitBootClocks();
}
/*${function:end}*/
