/*
 * Copyright 2026 NXP
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
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
	
	/* Attach peripheral clock */
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPSPI1);
    CLOCK_SetClockDiv(kCLOCK_DivLPSPI1, 1u);
}

uint32_t LPSPI1_GetFreq()
{
    return EXAMPLE_LPSPI_CLOCK_FREQ;
}
/*${function:end}*/
