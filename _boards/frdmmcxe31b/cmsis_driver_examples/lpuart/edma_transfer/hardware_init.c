/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{   
    BOARD_ConfigMPU();
    BOARD_InitBootClocks();

    /* Need explicitly enable TCD */
    CLOCK_EnableClock(kCLOCK_Tcd16);
    CLOCK_EnableClock(kCLOCK_Tcd17);

    /* DMAMux init and EDMA init */
    DMAMUX_Init(EXAMPLE_USART_DMAMUX_BASEADDR);
    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(EXAMPLE_USART_DMA_BASEADDR, &edmaConfig);
}

uint32_t LPUART5_GetFreq()
{
    return DEMO_LPUART_CLK_FREQ;
}
/*${function:end}*/
