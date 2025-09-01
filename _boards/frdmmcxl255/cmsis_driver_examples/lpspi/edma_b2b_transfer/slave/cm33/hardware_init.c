/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    BOARD_InitSPIPins();

    /* Enable DMA0 requests for LPSPI1 Receive and LPSPI1 Transmit */
    AHBSC__AHBSC0->SEC_GP_REG[0U] |= (1UL << kDma0RequestLPSPI1Tx);
    AHBSC__AHBSC0->SEC_GP_REG[0U] |= (1UL << kDma0RequestLPSPI1Rx);
}

uint32_t LPSPI1_GetFreq()
{
    return EXAMPLE_LPSPI_CLOCK_FREQ;
}
/*${function:end}*/