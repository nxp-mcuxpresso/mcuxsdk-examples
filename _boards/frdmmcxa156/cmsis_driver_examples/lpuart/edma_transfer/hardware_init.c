/*
 * Copyright 2024 NXP
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

    /* Release peripheral RESET */
    RESET_PeripheralReset(kDMA_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_GateDMA);
     
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(EXAMPLE_USART_DMA_BASEADDR, &edmaConfig);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART0_DMA_TX_CH, RTE_USART0_DMA_TX_PERI_SEL);
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART0_DMA_RX_CH, RTE_USART0_DMA_RX_PERI_SEL);
#endif
}

uint32_t LPUART0_GetFreq()
{
    return DEMO_LPUART_CLK_FREQ;
}
/*${function:end}*/
