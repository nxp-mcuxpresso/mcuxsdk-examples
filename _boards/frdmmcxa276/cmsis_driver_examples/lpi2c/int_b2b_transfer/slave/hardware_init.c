/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Release peripheral RESET */
    RESET_PeripheralReset(kDMA0_RST_SHIFT_RSTn);
    
    /* Attach peripheral clock */
    CLOCK_SetupFRO12MClocking();
    CLOCK_SetClockDiv(kCLOCK_DivLPI2C2, 1u);
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPI2C2);
    
    /* Enable DMA clock. */
    CLOCK_EnableClock(kCLOCK_GateDMA0);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

uint32_t LPI2C2_GetFreq(void)
{
    return LPI2C_CLOCK_FREQUENCY;
}
/*${function:end}*/
