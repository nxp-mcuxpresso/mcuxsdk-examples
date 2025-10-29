/*
 * Copyright 2023-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include "app.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    
    /* Enable DMA0 requests for LPUART0 Receive and LPUART0 Transmit */
    AHBSC__AHBSC0->SEC_GP_REG[0U] |= (1UL << kDma0RequestLPUART0Tx);
    AHBSC__AHBSC0->SEC_GP_REG[0U] |= (1UL << kDma0RequestLPUART0Rx);
}
/*${function:end}*/