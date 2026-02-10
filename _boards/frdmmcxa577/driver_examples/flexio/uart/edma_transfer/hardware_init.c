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
    BOARD_InitFLEXIO_UARTPins();
    BOARD_InitBootClocks();

    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivFLEXIO0, 1u);
    CLOCK_AttachClk(kPll1ClkDiv_to_FLEXIO0);

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kFLEXIO0_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kDMA0_RST_SHIFT_RSTn);

    /* Enable DMA0 request for FLEXIO */
    EDMA_SocRequestEnable(EXAMPLE_FLEXIO_UART_DMA_BASEADDR, EXAMPLE_TX_DMA_SOURCE, true);
    EDMA_SocRequestEnable(EXAMPLE_FLEXIO_UART_DMA_BASEADDR, EXAMPLE_RX_DMA_SOURCE, true);
}
/*${function:end}*/
