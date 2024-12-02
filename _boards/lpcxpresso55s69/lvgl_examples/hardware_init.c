/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_dma.h"
/*${header:end}*/

/*${macro:start}*/
#define EXAMPLE_LPSPI_MASTER_DMA_BASEADDR DMA0
/*${macro:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();
    DMA_Init(EXAMPLE_LPSPI_MASTER_DMA_BASEADDR);
}
/*${function:end}*/
