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

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_BootClockRUN();
}

extern uart_handle_t UART2_Handle;

void UART2_FLEXIO_IRQHandler(void)
{
    extern uart_handle_t UART2_Handle;
    UART_TransferHandleIRQ(UART2, &UART2_Handle);
}

uint32_t UART2_GetFreq(void)
{
    return CLOCK_GetFreq(UART2_CLK_SRC);
}
/*${function:end}*/
