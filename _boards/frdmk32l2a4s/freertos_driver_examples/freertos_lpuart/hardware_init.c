/*
 * Copyright 2019 NXP
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
    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSircAsync);
    NVIC_SetPriority(DEMO_LPUART_RX_TX_IRQn, 3);
}
/*${function:end}*/
