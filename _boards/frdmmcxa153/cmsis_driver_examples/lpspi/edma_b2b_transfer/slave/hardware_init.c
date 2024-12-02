/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Release peripheral RESET */
    RESET_PeripheralReset(kPORT1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kPORT2_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kLPSPI1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kDMA_RST_SHIFT_RSTn);
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivLPSPI1, 1u);
    CLOCK_AttachClk(kFRO12M_to_LPSPI1);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

uint32_t LPSPI1_GetFreq()
{
    return EXAMPLE_LPSPI_CLOCK_FREQ;
}
/*${function:end}*/
