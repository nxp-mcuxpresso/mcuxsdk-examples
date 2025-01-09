/*
 * Copyright 2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();

    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Use aux0_pll_clk / 2 */
    BOARD_SetFlexspiClock(FLEXSPI, 2U, 2U);

    CLOCK_EnableClock(kCLOCK_Flexspi);
    RESET_ClearPeripheralReset(kFLEXSPI_RST_SHIFT_RSTn);
}
/*${function:end}*/
