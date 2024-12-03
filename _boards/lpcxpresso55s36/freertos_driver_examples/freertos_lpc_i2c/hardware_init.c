/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach 12 MHz clock to FLEXCOMM7 (I2C master) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom7Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom7Clk, 1u, true);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM7);

    /* attach 12 MHz clock to FLEXCOMM2 (I2C slave) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u, true);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC7_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kFC2_RST_SHIFT_RSTn);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Set interrupt priorities */
    NVIC_SetPriority(EXAMPLE_I2C_SLAVE_IRQ, 2);
    NVIC_SetPriority(EXAMPLE_I2C_MASTER_IRQ, 3);
}
/*${function:end}*/
