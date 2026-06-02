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
#include "fsl_edma_soc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitCANPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_AttachClk(kFRO_HF_DIV_to_FLEXCAN0);
    CLOCK_SetClockDiv(kCLOCK_DivFLEXCAN0, 1U);
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF, 4U);
    
    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kDMA0_RST_SHIFT_RSTn);

    /* Enable DMA0 request for FLEXCAN */
    EDMA_SocRequestEnable(EXAMPLE_CAN_DMA, FLEXCAN_DMA_REQUEST_SOURCE, true);
}
/*${function:end}*/
