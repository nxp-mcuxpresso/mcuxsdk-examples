/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/

/* Initialize debug console. */
void APP_InitDebugConsole(void)
{
    /* attach 12 MHz clock to LPUART3 (debug console) */
    CLOCK_SetClockDiv(kCLOCK_DivLPUART3, 1u);
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPUART3);

    RESET_PeripheralReset(kLPUART3_RST_SHIFT_RSTn);

    DbgConsole_Init(3U, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    BOARD_DEBUG_UART_CLK_FREQ);
}

void BOARD_InitHardware(void)
{
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivDAC0, 1u);
    CLOCK_AttachClk(kFRO_LF_DIV_to_DAC0);

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kDAC0_RST_SHIFT_RSTn);

    /* Enable DAC0 */
    SPC0->ACTIVE_CFG1 |= 0x10;

    BOARD_InitPins();
    BOARD_InitBootClocks();
    APP_InitDebugConsole();
}
/*${function:end}*/
