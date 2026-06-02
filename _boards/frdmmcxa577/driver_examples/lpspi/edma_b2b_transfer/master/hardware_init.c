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

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitSPIPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPSPI1);
    CLOCK_SetClockDiv(kCLOCK_DivLPSPI1, 1u);

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kDMA0_RST_SHIFT_RSTn);

    /* Enable DMA0 request for LPSPI */
    EDMA_SocRequestEnable(EXAMPLE_LPSPI_MASTER_DMA_BASE, DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL, true);
    EDMA_SocRequestEnable(EXAMPLE_LPSPI_MASTER_DMA_BASE, DEMO_LPSPI_RECEIVE_EDMA_CHANNEL, true);
}
/*${function:end}*/
