/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    //BOARD_InitBootPins();
    NVIC_EnableIRQ(MU_B_TX_IRQn);
    NVIC_EnableIRQ(MU_B_RX_IRQn);
    NVIC_EnableIRQ(MU_B_INT_IRQn);
}
/*${function:end}*/
