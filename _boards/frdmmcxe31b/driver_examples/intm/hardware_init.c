/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitEXTIRQPins();
    BOARD_InitBootClocks();

    BOARD_InitDebugConsole();
}

void BOARD_InitSWConfig(void)
{
   SIUL2_EnableExtInterrupt(SIUL2, BOARD_SW_EIRQ, kSIUL2_InterruptFallingEdge, 0);
   EnableIRQ(BOARD_SW_IRQ);
}

void BOARD_ClearSWIntFlag(void)
{
   SIUL2_ClearExtDmaInterruptStatusFlags(SIUL2, 1U << BOARD_SW_EIRQ);
}
/*${function:end}*/
