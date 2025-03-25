/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include "app.h"
#include "fsl_mu.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitSWD_DEBUGPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    
    
    EnableIRQ(MU_B_RX_IRQn);
    /* Enable transmit and receive interrupt */
    MU_EnableInterrupts(APP_MU, (kMU_Rx0FullInterruptEnable | kMU_Tx3EmptyInterruptEnable));
}
/*${function:end}*/
