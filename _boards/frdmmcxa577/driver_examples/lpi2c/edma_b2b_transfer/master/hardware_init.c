/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_edma_soc.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitI2CPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivLPI2C0, 1u);
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPI2C0);

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kDMA0_RST_SHIFT_RSTn);

    /* Enable DMA0 request for LPI2C0 */
    EDMA_SocRequestEnable(EXAMPLE_LPI2C_MASTER_DMA, DEMO_LPI2C_TRANSMIT_EDMA_CHANNEL, true);
    EDMA_SocRequestEnable(EXAMPLE_LPI2C_MASTER_DMA, DEMO_LPI2C_RECEIVE_EDMA_CHANNEL, true);
}
/*${function:end}*/
