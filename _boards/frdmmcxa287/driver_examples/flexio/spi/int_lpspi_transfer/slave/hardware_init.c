/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitFLEXIO_SPIPins();
    BOARD_InitSPIPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_AttachClk(kFRO_HF_to_FLEXIO0);
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPSPI2);

    CLOCK_SetClockDiv(kCLOCK_DivFLEXIO0, 2u);
    CLOCK_SetClockDiv(kCLOCK_DivLPSPI2, 1u);

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kFLEXIO0_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kLPSPI2_RST_SHIFT_RSTn);
}
/*${function:end}*/
