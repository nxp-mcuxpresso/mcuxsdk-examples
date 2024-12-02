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
    RESET_PeripheralReset(kPORT3_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kLPI2C3_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kDMA_RST_SHIFT_RSTn);
    CLOCK_SetupFRO12MClocking();
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivLPI2C3, 1u);
    CLOCK_AttachClk(kFRO12M_to_LPI2C3);
    
    /* Enable DMA clock. */
    CLOCK_EnableClock(kCLOCK_GateDMA);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

uint32_t LPI2C3_GetFreq(void)
{
    return LPI2C_CLOCK_FREQUENCY;
}
/*${function:end}*/
