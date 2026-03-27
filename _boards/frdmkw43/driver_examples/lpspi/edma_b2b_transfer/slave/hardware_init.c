/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitFlashPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(EXAMPLE_LPSPI_SLAVE_CLOCK_NAME, EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE);
    CLOCK_SetIpSrcDiv(EXAMPLE_LPSPI_SLAVE_CLOCK_NAME, 1U);
    CLOCK_EnableClock(kCLOCK_Fro_hf_div);
    /* Enable DMA0 clock */
    CLOCK_EnableClock(kCLOCK_Dma0);
}
/*${function:end}*/
