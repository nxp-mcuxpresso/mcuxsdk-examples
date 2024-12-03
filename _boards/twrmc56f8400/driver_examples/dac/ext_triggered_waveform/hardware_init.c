/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "fsl_xbara.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    BOARD_InitDebugConsole();

    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_ERR_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TX_IDLE_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);

    XBARA_Init(DEMO_XBARA_BASE);
    /* Input a 1 kHz external clock signal and connect it with SYNC_IN of DAC*/
    XBARA_SetSignalsConnection(DEMO_XBARA_BASE, DEMO_XBARA_XB_CHANNEL, DEMO_XBARA_DAC_CHANNEL);

    SetIRQBasePriority(0);
}
/*${function:end}*/
