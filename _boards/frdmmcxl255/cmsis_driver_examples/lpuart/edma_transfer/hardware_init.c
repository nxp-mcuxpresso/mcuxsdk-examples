/*
 * Copyright 2025 NXP
 * All rights reserved.
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
    CLOCK_EnableClock(kCLOCK_GateDMA);
    CLOCK_EnableClock(kCLOCK_GateLPUART2);
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP1);

    /* Release peripheral RESET */
    RESET_PeripheralReset(kDMA_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kPORT1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kLPUART2_RST_SHIFT_RSTn);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(EXAMPLE_USART_DMA_BASEADDR, &edmaConfig);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART2_DMA_TX_CH, RTE_USART2_DMA_TX_PERI_SEL);
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART2_DMA_RX_CH, RTE_USART2_DMA_RX_PERI_SEL);
#endif
}

uint32_t LPUART2_GetFreq()
{
    return DEMO_LPUART_CLK_FREQ;
}
/*${function:end}*/
