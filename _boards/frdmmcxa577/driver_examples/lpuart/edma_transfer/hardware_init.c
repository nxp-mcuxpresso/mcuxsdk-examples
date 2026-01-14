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
    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kDMA0_RST_SHIFT_RSTn);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    
    /* Enable DMA0 request for LPUART_TX and LPUART_RX */
    EDMA_SocRequestEnable(DMA0, DEMO_LPUART_TX_EDMA_CHANNEL, true);
    EDMA_SocRequestEnable(DMA0, DEMO_LPUART_RX_EDMA_CHANNEL, true);
}
/*${function:end}*/
