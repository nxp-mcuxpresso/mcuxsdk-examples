/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_inputmux.h"
#include "board.h"
#include "app.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    
    /* TODO: Add to pin_mux - Connect pin set as CTimer0 to DMA0 */
    INPUTMUX_Init(CTRLSS__INPUTMUX_0);
    INPUTMUX_AttachSignal(CTRLSS__INPUTMUX_0, kINPUTMUX_INDEX_DMA0_ITRIG_INMUX0, kINPUTMUX_DmaM0PBreqATimer0ToDma0ItrigInmuxn);
}
/*${function:end}*/