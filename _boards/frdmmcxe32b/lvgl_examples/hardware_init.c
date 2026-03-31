/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_siul2.h"
#include "clock_config.h"
#include "app.h"
#include "lvgl_support.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitLCDPins();
}

void BOARD_LCD_INT_IRQHandler(void)
{
    uint32_t flags = SIUL2_GetExtDmaInterruptStatusFlags(SIUL2);

    SIUL2_ClearExtDmaInterruptStatusFlags(SIUL2, flags);

    if ((flags & (1UL << BOARD_LCD_INT_EIRQ)) != 0U)
    {
        BOARD_TouchIntHandler();
    }

    SDK_ISR_EXIT_BARRIER;
}
/*${function:end}*/
