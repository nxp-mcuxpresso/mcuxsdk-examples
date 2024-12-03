/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_uart_freertos.h"
/*${header:end}*/

extern uart_rtos_handle_t handle;
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    NVIC_SetPriority(DEMO_UART_RX_TX_IRQn, 5);
}

void UART2_FLEXIO_IRQHandler(void)
{
    UART_TransferHandleIRQ(UART2, (uart_handle_t *)(handle.t_state));
}
/*${function:end}*/
