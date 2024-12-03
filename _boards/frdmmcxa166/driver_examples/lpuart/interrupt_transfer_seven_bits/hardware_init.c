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
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivLPUART3, 1u);
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPUART3);

    BOARD_InitPins();
    BOARD_InitBootClocks();
}
/*${function:end}*/
