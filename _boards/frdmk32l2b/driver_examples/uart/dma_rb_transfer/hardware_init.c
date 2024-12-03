/*
 * Copyright 2019,2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_uart_dma.h"
/*${header:end}*/

extern uart_dma_handle_t g_uartDmaHandle;
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
}

/*${function:end}*/
