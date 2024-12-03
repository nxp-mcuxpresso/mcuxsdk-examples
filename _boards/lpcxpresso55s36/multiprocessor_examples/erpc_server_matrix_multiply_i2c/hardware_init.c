/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach 12 MHz clock to I2C1 */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom1Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom1Clk, 1u, true);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM1);

    /* reset FLEXCOMM for SPI */
    RESET_PeripheralReset(kFC1_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_BootClockPLL150M();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
