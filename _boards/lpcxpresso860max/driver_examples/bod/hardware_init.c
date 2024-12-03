/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_power.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Attach 12 MHz clock to USART0 (debug console) */
    CLOCK_Select(BOARD_DEBUG_USART_CLK_ATTACH);
    BOARD_InitPins();
    BOARD_BootClockFRO48M();
    BOARD_InitDebugConsole();
}

void APP_InitBod(void)
{
    POWER_DisablePD(kPDRUNCFG_PD_BOD);
    /* software delay 30USs */
    SDK_DelayAtLeastUs(30U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    NVIC_ClearPendingIRQ(APP_BOD_IRQn);
    POWER_SetBodLevel(kBod_ResetLevelReserved, kBod_InterruptLevel2, false);
}

void APP_DeinitBod(void)
{
    POWER_SetBodLevel(kBod_ResetLevelReserved, kBod_InterruptLevelReserved, false);
}
/*${function:end}*/
