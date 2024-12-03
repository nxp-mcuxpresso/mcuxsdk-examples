/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_uart.h"
/*${header:end}*/

extern uart_handle_t g_uartHandle;
/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
}

void UART2_FLEXIO_IRQHandler(void)
{
    UART_TransferHandleIRQ(UART2, &g_uartHandle);
}
/*${function:end}*/
