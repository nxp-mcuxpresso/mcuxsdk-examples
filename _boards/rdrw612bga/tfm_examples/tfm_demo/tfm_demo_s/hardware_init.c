/*
 * Copyright 2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // ---------------  flexcomm14 clock---------------
    /* Configure FRG14 (choose source & fractional divider) */
    CLOCK_SetFRGClock(&(const clock_frg_clk_config_t){14, kCLOCK_FrgPllDiv, 255, 0});
    
    /* Attach FRG14 output to FLEXCOMM14 function clock */
    RESET_PeripheralReset(kFC14_RST_SHIFT_RSTn);
    RESET_ClearPeripheralReset(kFC14_RST_SHIFT_RSTn);
    
    /* Read back/verify the clock frequency for FC14 */
    CLOCK_AttachClk(kFRG_to_FLEXCOMM14);
    
    /* Gate on the FLEXCOMM14 peripheral clock */
    CLOCK_EnableClock(kCLOCK_Flexcomm14);
    // --------------- flexcomm14 end---------------    
}
/*${function:end}*/
