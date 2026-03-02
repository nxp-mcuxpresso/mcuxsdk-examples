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
#include "RTE_Device.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Release peripheral RESET */
    RESET_PeripheralReset(kDMA0_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_GateDMA0);

    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(EXAMPLE_USART_DMA_BASEADDR, &edmaConfig);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART1_DMA_TX_CH, RTE_USART1_DMA_TX_PERI_SEL);
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART1_DMA_RX_CH, RTE_USART1_DMA_RX_PERI_SEL);
#endif

    /* Enable DMA0 request for LPUART */
    EDMA_SocRequestEnable(RTE_USART1_DMA_TX_DMA_BASE, RTE_USART1_DMA_TX_PERI_SEL, true);
    EDMA_SocRequestEnable(RTE_USART1_DMA_RX_DMA_BASE, RTE_USART1_DMA_RX_PERI_SEL, true);
}

uint32_t LPUART1_GetFreq()
{
    return DEMO_LPUART_CLK_FREQ;
}
/*${function:end}*/
