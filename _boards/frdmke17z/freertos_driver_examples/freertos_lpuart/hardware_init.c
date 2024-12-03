/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSysOscAsync);
    NVIC_SetPriority(LPUART0_IRQn, 2U);
}
/*${function:end}*/
