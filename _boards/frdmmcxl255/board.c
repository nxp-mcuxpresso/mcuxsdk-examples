/*
 * Copyright 2023,2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdint.h>
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_reset.h"
//#include "fsl_ipmq.h"
//#include "fsl_aon_common.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    /* attach 12 MHz clock to LPUART2 (debug console) */
    //CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    //CLOCK_SetClockDiv(kCLOCK_DivLPUART2, 1u);

    RESET_PeripheralReset(BOARD_DEBUG_UART_RST);

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    BOARD_DEBUG_UART_CLK_FREQ);
}
