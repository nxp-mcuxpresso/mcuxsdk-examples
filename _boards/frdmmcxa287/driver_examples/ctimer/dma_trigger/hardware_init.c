/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include <stdbool.h>
#include "app.h"
#include "fsl_edma_soc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_AttachClk(kFRO_HF_to_CTIMER0);
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER0, 1u);
    
    /* Enable DMA0 request for CTIMER */
    EDMA_SocRequestEnable(EXAMPLE_DMA_BASEADDR, DEMO_DMA_REQUEST_SOURCE, true);
}
/*${function:end}*/
