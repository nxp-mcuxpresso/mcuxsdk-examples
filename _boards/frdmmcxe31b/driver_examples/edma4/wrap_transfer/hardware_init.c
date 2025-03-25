/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_dmamux.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Need explicitly enable TCD clock. */
    CLOCK_EnableClock(kCLOCK_Tcd0);

    /* DMAMUX init */
    DMAMUX_Init(EXAMPLE_EDMA_DMA_MUX);
    DMAMUX_SetSource(EXAMPLE_EDMA_DMA_MUX, EXAMPLE_DMA_CHANNEL, EXAMPLE_ALWAYS_ON_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_EDMA_DMA_MUX, EXAMPLE_DMA_CHANNEL);
}
/*${function:end}*/
